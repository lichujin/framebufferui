#include "screen.h"
#include "MessageBox.h"
#include "include/asc24x48.h"
#include "include/types.h"
#include "Mouse.h"
#include "Termios.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>


#include <iostream>
#include <vector>

using namespace std;

int screen::x = 0;
int screen::y = 0;
int screen::exit = 0;
int screen::xres;
int screen::yres;
int screen::xres_virtual;
int screen::yres_virtual;
unsigned int * screen::mem;
int screen::fd;
vector<Window *> screen::child;
StatusBar screen::statusbar;
int screen::activewin = 0xFF;
view * screen::focus_view;


int screen::init()
{
	int ret = 0;
	struct fb_var_screeninfo vinfo;

	fd = open(DEVICE,O_RDWR);
	if(screen::fd < 0)
	{
		printf("Open %s failed\n",DEVICE);
		return -1;
	}
	if (ioctl(screen::fd,FBIOGET_VSCREENINFO,&vinfo)){
		printf("Get device info failed\n");
		return -2;
	}

	screen::xres = vinfo.xres;
	screen::yres = vinfo.yres;
	screen::xres_virtual = vinfo.xres_virtual;
	screen::yres_virtual = vinfo.yres_virtual;
	mem = (unsigned int *)mmap(NULL,xres*yres*4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);



	#ifdef INPUT_MOUSE
	Mouse::start();
	#endif

	#ifdef INPUT_TERMIOS
	Termios::start();
	#endif

	statusbar.OnDraw();
	return ret;
}


int screen::de_init()
{
	int ret = 0;

	while(!exit);

	ret = munmap(mem,xres*yres*4);
	close(fd);

	#ifdef INPUT_MOUSE
	close(Mouse::fd);
	pthread_join(Mouse::dev,NULL);
	#endif
	#ifdef INPUT_TERMIOS
	tcsetattr(0,TCSANOW,&Termios::save);
	pthread_cancel(Termios::dev);
	#endif

	return ret;
}
int screen::draw_pixel(int x1,int y1,unsigned int color)
{
	int ret = 0;

	*(mem +y1*xres+x1) = color;

	return ret;
}
int screen::draw_line_(int x1,int y1,int len,unsigned int color)
{

	int ret = 0;
	int i;

	for(i = x1;i<(x1+len);i++)
		draw_pixel(i,y1,color);

	return ret;
}

int screen::draw_linel(int x1,int y1,int len,unsigned int color)
{

	int ret = 0;
	int i;

	for(i=y1;i<(y1+len);i++)
		draw_pixel(x1,i,color);

	return ret;
}
int screen::draw_rect(int x1,int y1,int x2,int y2,unsigned int color)
{

	int ret = 0;
	int len1,len2;

	len1 = x2-x1;
	len2 = y2-y1;

	draw_line_(x1,y1,len1,color);
	draw_line_(x1,y2,len1,color);

	draw_linel(x1,y1,len2,color);
	draw_linel(x2,y1,len2,color);

	return ret;

}
int screen::fill_rect(int x1,int y1,int x2,int y2,unsigned int color)
{
	int ret = 0;
	int i,j;

	for(i=x1;i<x2;i++)
	for(j=y1;j<y2;j++)
		draw_pixel(i,j,color);

	return ret;
}
int screen::draw_circle(int x1,int y1,int r,unsigned int color)
{
	double angle = 0;
	int xpos,ypos;

	for(angle = 0;angle<=6.28;angle+=3.14/180)
	{
		xpos = x1 + (int)cos(angle)*r;
		ypos = y1 - (int)sin(angle)*r;
		draw_pixel(xpos,ypos,color);
	}
	return 0;
}
int screen::draw_char(int x1,int y1,const unsigned char a,unsigned int color){
	int ret = 0;
	int offset = (a-32)*CHAR_WIDTH*CHAR_HEIGHT/8;
	int i,j,k;

	for(i = 0;i < CHAR_HEIGHT;i++){
	//for(i = CHAR_HEIGHT;i > 0;i--){
		for(j=0;j<CHAR_WIDTH/8;j++){
			for(k=0;k<8;k++){
				if((asc24x48[offset]>>(8-k))&1)
					ret = draw_pixel((x1+j*8+k),(y1+i),color);
			}
			offset ++;
		}
	}
	return ret;
}

int screen::draw_string(int x1,int y1,string s,unsigned int color){
	int ret = 0;
	const char *text = s.c_str();
	int len = strlen(text);
	int i;

	for(i = 0;i<len;i++)
		draw_char((x1+i*24+2),y1,(const unsigned char)text[i],color);

	return ret;
}

int screen::draw_exit_icon(int x1,int y1,unsigned int color)
{
	int i,j,len = 5;

	for(i = 0;i<len;i++){
		*(screen::mem+(y1+i)*screen::xres+x1+i) = color;
		*(screen::mem+(y1-i)*screen::xres+x1+i) = color;
	}
	for(i = 0;i<len;i++){
		*(screen::mem+(y1+i)*screen::xres+x1-i) = color;
		*(screen::mem+(y1-i)*screen::xres+x1-i) = color;
	}
	return 0;
}

int screen::draw_min_icon(int x1,int y1,unsigned int color)
{
	int len = 10;
	draw_line_((x1+1),(y1-1),(len-2),color);
	draw_line_(x1,y1,len,color);
	draw_line_((x1+1),(y1+1),(len-2),color);
	return 0;
}

int screen::draw_image(int x1,int y1,string file,int alpha){
	bmphead head;
	int fd;
	int i,j;
	unsigned char r,g,b;
	int color;

	const char *name = file.c_str();

	fd = open(name,O_RDONLY);
	if(fd<0){
		MessageBox::show("Error","Open Image file failed",5);
		return -1;
	}

	read(fd,&head,sizeof(head));

	if((head.filetype!=19778)&&(head.bfoffset!=54)&&
		(head.bisize!=40)&&(head.bibitcount!=24)){
		MessageBox::show("Error","BMP File Not correct",5);
		close(fd);
		return -2;
	}

	if(head.biwidth+x>screen::xres||head.biheight+y>screen::yres){
		MessageBox::show("Error","BMP File is too big",5);
		return -2;
	}
	for(i = y1+head.biheight;i>y1;i--)
	for(j = x1;j<head.biwidth+x1;j++)
	{
		read(fd,&b,1);
		read(fd,&g,1);
		read(fd,&r,1);
		r = r*alpha/255+(*(screen::mem+i*screen::xres+j)&0xFF0000)*(255-alpha)/255;
		g = g*alpha/255+(*(screen::mem+i*screen::xres+j)&0xFF00)*(255-alpha)/255;
		b = b*alpha/255+(*(screen::mem+i*screen::xres+j)&0xFF)*(255-alpha)/255;
		color = r<<16 |g<<8 |b;

		//read(fd,&color,3);
		*(screen::mem+i*screen::xres+j) = color;
	}

	close(fd);
	return 0;
}


int screen::utf8_ucde(unsigned char *tmp){
	//utf-8:1110xxxx 10xxxxxx 10xxxxxx 
	int ret = 0;
	unsigned char a,b,c;

	a = tmp[0]&0xF;//0-3

	b = tmp[1]&0x3F;//0-5

	c = tmp[2]&0x3F;//0-5


	ret = a<<12;
	ret = ret | (b<<6);
	ret = ret | c;


	return ret;
}

int screen::posinrect(int x1,int y1,int x2,int y2){
	if(x>x1&&x<x2&&y>y1&&y<y2)
		return 1;
	else
		return 0;
}

int screen::add(Window *v)
{
	child.push_back(v);
	statusbar.OnDraw();
	return child.size();
}

int screen::del(Window *v)
{
	vector<Window *>::iterator it;

	for(it = child.begin();it!=child.end();it++)
	if(*it == v){
		it = child.erase(it);
		break;
	}

	statusbar.OnDraw();
	return 0;
}

int screen::sendmessage(int type,int value){

	switch(type){
		case EVENT_LEFT_CLICK:
		{
			if(posinrect(0,screen::yres-STATUSBAR_HEIGHT,screen::xres,screen::yres))
				statusbar.OnClick();
			else
				if(screen::activewin != 0xFF)
					screen::child[screen::activewin]->OnClick();
			break;

		}
		case EVENT_RIGHT_CLICK:
		{
			exit = 1;
			break;
		}
		case EVENT_MOUSE_MOVE:
		{
			if(screen::activewin != 0xFF)
				screen::child[screen::activewin]->OnFocus();
			break;
		}
		case EVENT_KEY_PRESS:
		{
			focus_view->OnKeyPress(value);
			break;
		}
		default:
		break;
	}
	return 0;
}
