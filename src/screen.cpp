#include "screen.h"
#include "MessageBox.h"
#include "asc24x48.h"
#include "types.h"
#include "Mouse.h"
#include "Termios.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <termios.h>


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
#ifdef CONFIG_FREETYPE
FT_Library screen::ftLibrary;
FT_Face screen::ftFace;
#endif
#ifdef CONFIG_PNG
png_structp screen::png_ptr;
png_infop screen::info_ptr;
#endif


#ifdef CONFIG_JPEG
struct jpeg_decompress_struct screen::cinfo;
struct my_error_mgr screen::jerr;
static void my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);

  longjmp(myerr->setjmp_buffer, 1);
}
#endif


int screen::init()
{
	int ret = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;

	fd = open(CONFIG_DISPLAY_DEVICE,O_RDWR);
	if(screen::fd < 0)
	{
		printf("Open %s failed\n",CONFIG_DISPLAY_DEVICE);
		return -1;
	}
	if (ioctl(fd,FBIOGET_FSCREENINFO,&finfo)){
		printf("Get device fix info failed\n");
		return -2;
	}
	if (ioctl(screen::fd,FBIOGET_VSCREENINFO,&vinfo)){
		printf("Get device info failed\n");
		return -2;
	}

	screen::yres = vinfo.yres;
	screen::yres_virtual = vinfo.yres_virtual;
	screen::xres = vinfo.xres;
	screen::xres_virtual = finfo.line_length/4;
	mem = (unsigned int *)mmap(NULL,xres_virtual*yres_virtual*4,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);


	#ifdef CONFIG_INPUT_MOUSE
	Mouse::start();
	#endif

	#ifdef CONFIG_TERMIOS
	Termios::start();
	#endif

	#ifdef CONFIG_FREETYPE
	FT_Error ftError = FT_Init_FreeType(&ftLibrary);
	if(ftError){
		MessageBox::show("Error","Init FreeType library failed",5);
		return -3;
	}
	ftError = FT_New_Face( ftLibrary, CONFIG_CHARACTER_PATH, 0, &ftFace );
	if(ftError == FT_Err_Unknown_File_Format)
	{
		MessageBox::show("Error","Unknown file format",5);
	    	return -3;
	}
	else if(ftError)
	{
		MessageBox::show("Error","Load file failed",5);
	    	return -3;
	}
	ftError = FT_Set_Pixel_Sizes(ftFace, CHAR_WIDTH, 0);
	if(ftError)
	{
		MessageBox::show("Error","Set Char size failed",5);
	    	return -3;
	}

	#endif

	#ifdef CONFIG_PNG
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
	if (png_ptr == NULL){
		MessageBox::show("Error","Create png library failed",5);
		return -4;
	}

	info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL)
        {
            	png_destroy_read_struct(&png_ptr, NULL, NULL);
		MessageBox::show("Error","Create png info failed",5);
            	return -4;
        }
	#endif

	#ifdef CONFIG_JPEG
 	cinfo.err = jpeg_std_error(&jerr.pub);
  	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		return -1;
	}

	jpeg_create_decompress(&cinfo);
	#endif

	statusbar.OnDraw();
	return ret;
}


int screen::de_init()
{
	int ret = 0;

	while(!exit);

	ret = munmap(mem,xres_virtual*yres_virtual*4);
	close(fd);

	#ifdef CONFIG_INPUT_MOUSE
	Mouse::destroy();
	#endif
	#ifdef CONFIG_TERMIOS
	Termios::destroy();
	#endif

	#ifdef CONFIG_PNG
	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	#endif

	#ifdef CONFIG_JPEG
	jpeg_destroy_decompress(&cinfo);
	#endif
	return ret;
}
int screen::draw_pixel(int x1,int y1,unsigned int color)
{
	int ret = 0;

	*(mem +y1*xres_virtual+x1) = color;

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

	len1 = x2-x1+1;
	len2 = y2-y1+1;

	draw_line_(x1,y1,len1,color);
	draw_line_(x1,y2,len1,color);

	draw_linel(x1,y1,len2,color);
	draw_linel(x2,y1,len2,color);

	return ret;

}
int screen::draw_roundrect(int x1,int y1,int x2,int y2,int r,unsigned int color)
{
	draw_line_(x1+r,y1,x2-x1-r*2,color);
	draw_line_(x1+r,y2,x2-x1-r*2,color);

	draw_linel(x1,y1+r,y2-y1-r*2,color);
	draw_linel(x2,y1+r,y2-y1-r*2,color);

	draw_arc(x1+r,y1+r,r,90,180,color);
	draw_arc(x2-r,y1+r,r,0,90,color);
	draw_arc(x1+r,y2-r,r,180,270,color);
	draw_arc(x2-r,y2-r,r,270,360,color);
	
	return 0;
}
int screen::fill_rect(int x1,int y1,int x2,int y2,unsigned int color)
{
	int ret = 0;
	int i,j;

	for(i=x1+1;i<x2;i++)
	for(j=y1+1;j<y2;j++)
		draw_pixel(i,j,color);

	return ret;
}
int screen::fill_roundrect(int x1,int y1,int x2,int y2,int r,unsigned int color)
{
	int i;

	for(i=1;i<=r;i++){
		draw_roundrect(x1+i,y1+i,x2-i,y2-i,r,color);
	}

	fill_circle(x1+r,y1+r,r,color);
	fill_circle(x2-r,y1+r,r,color);
	fill_circle(x1+r,y2-r,r,color);
	fill_circle(x2-r,y2-r,r,color);
	
	fill_rect(x1+r,y1+r,x2-r,y2-r,color);
	return 0;
}
int screen::draw_circle(int x1,int y1,int r,unsigned int color)
{
	double angle = 0;
	int xpos,ypos;

	for(angle = 0;angle<=6.28;angle+=3.14/180)
	{
		xpos = x1 + (int)(cos(angle)*r);
		ypos = y1 - (int)(sin(angle)*r);
		draw_pixel(xpos,ypos,color);
	}
	return 0;
}
int screen::fill_circle(int x1,int y1,int r,unsigned int color)
{
	int i;
	for(i=r;i>0;i--)
		draw_circle(x1,y1,i,color);
	return 0;
}
int screen::draw_arc(int x1,int y1,int r,int start,int end,unsigned int color)
{
	double angle = 0;
	int xpos,ypos;

	for(angle = start/360.0*6.28;angle<=end/360.0*6.28;angle+=3.14/180)
	{
		xpos = x1 + (int)(cos(angle)*r);
		ypos = y1 - (int)(sin(angle)*r);
		draw_pixel(xpos,ypos,color);
	}
	
	return 0;
}
#ifdef CONFIG_FREETYPE
int screen::draw_char(int x1,int y1,unsigned int uiCharCode,unsigned int color){
	int iRow = 0, iCol = 0;
	FT_UInt uiGlyphIndex = FT_Get_Char_Index(ftFace, uiCharCode);
	FT_Load_Glyph(ftFace,uiGlyphIndex, FT_LOAD_DEFAULT);
	FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_MONO);

     	for(iRow = 0; iRow < ftFace->glyph->bitmap.rows; iRow++)
     	{
         	for(iCol = 0; iCol < ftFace->glyph->bitmap.width; iCol++)
         	{
              		if((ftFace->glyph->bitmap.buffer[iRow*ftFace->glyph->bitmap.pitch+iCol/8]&(0xC0>>(iCol%8)))==0)
              		{
              		}
   			else
            		{
				draw_pixel(x1+iCol,y1+iRow,color);
        		}
         	}
     	}
	return 0;
}
#else
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
#endif
int screen::draw_string(int x1,int y1,string s,unsigned int color){
	int ret = 0;
	const char *text = s.c_str();
	int len = strlen(text);
	int i;
	unsigned int unicode;
	int offset = 0;	

	for(i = 0;i<len;){
		#ifdef CONFIG_FREETYPE
		if((unsigned char)text[i]>=0x80){
			unicode = utf8_ucde((unsigned char *)&text[i]);
			draw_char((x1+offset+2),y1,unicode,color);
			i+=3;
		}else{
			unicode = (unsigned int)text[i];
			draw_char((x1+offset+2),y1,unicode,color);
			i++;
		}
		offset += ftFace->glyph->bitmap.width+2;
		#else
		draw_char((x1+i*24+2),y1,(const unsigned char)text[i],color);
		i++;
		#endif
	}

	return ret;
}

int screen::draw_exit_icon(int x1,int y1,unsigned int color)
{
	int i,j,len = 5;

	for(i = 0;i<len;i++){
		*(screen::mem+(y1+i)*screen::xres_virtual+x1+i) = color;
		*(screen::mem+(y1-i)*screen::xres_virtual+x1+i) = color;
	}
	for(i = 0;i<len;i++){
		*(screen::mem+(y1+i)*screen::xres_virtual+x1-i) = color;
		*(screen::mem+(y1-i)*screen::xres_virtual+x1-i) = color;
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
		r = r*alpha/255+(*(screen::mem+i*screen::xres_virtual+j)&0xFF0000)*(255-alpha)/255;
		g = g*alpha/255+(*(screen::mem+i*screen::xres_virtual+j)&0xFF00)*(255-alpha)/255;
		b = b*alpha/255+(*(screen::mem+i*screen::xres_virtual+j)&0xFF)*(255-alpha)/255;
		color = r<<16 |g<<8 |b;

		//read(fd,&color,3);
		*(screen::mem+i*screen::xres_virtual+j) = color;
	}

	close(fd);
	return 0;
}
#ifdef CONFIG_PNG
int screen::draw_png(int x1,int y1,string file)
{
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type, number_passes;
	const char *filename = file.c_str();
	FILE *fp;
	int size,i,j,row;
	unsigned int *tmp;

	if ((fp = fopen(filename, "rb")) == NULL)
        {
		MessageBox::show("Error","Open png file error",5);
          	return -1;
        }

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr,info_ptr,&width,&height,&bit_depth,&color_type,&interlace_type,NULL,NULL);
	number_passes = png_set_interlace_handling(png_ptr);

	if(width+x1>xres||height+y1>yres){
		MessageBox::show("Error","Png file reslution is too big",5);
		return -1;
	}

	png_bytep row_pointers[height];

        for (row = 0; row < height; row++){
                row_pointers[row] = NULL;
                row_pointers[row] = (png_bytep)malloc(width * 4);
                memset(row_pointers[row], 0, width * 4);
	}

	row = y1;
        for (i = 0; i < height; i++)
        {
                png_read_rows(png_ptr, &row_pointers[i], NULL, 1);
                tmp = (unsigned int *)row_pointers[i];
                memcpy(mem+row*xres+x1,tmp,width*4);
                row++;
        }

	for (row = 0; row < height; row++)
		free(row_pointers[row]);

	fclose(fp);

}
#endif
#ifdef CONFIG_JPEG
int screen::draw_jpeg(int x1,int y1,string file)
{
	const char *filename = file.c_str();
	FILE * infile;
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
 	unsigned long width;
 	unsigned long height;
 	unsigned short depth;
	unsigned char *src;
	unsigned int tmp;
	int i,j;


	if ((infile = fopen(filename, "rb")) == NULL) {
		MessageBox::show("Error","Open jpeg file error",5);
		return -1;
	}

 	jpeg_stdio_src(&cinfo, infile);
	(void) jpeg_read_header(&cinfo, TRUE);
	(void) jpeg_start_decompress(&cinfo);

 	width = cinfo.output_width;//图像宽度
 	height = cinfo.output_height;//图像高度
 	depth = cinfo.output_components;//图像深度

	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	src = (unsigned char *)malloc(width*depth);

	j = y1;
	while (cinfo.output_scanline < cinfo.output_height) {
		(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		memset(src,0,width*depth);
		memcpy(src,*buffer,width*depth);
		for(i=0;i<cinfo.output_width;i++)
		{
			tmp = (unsigned int)src[i*3];
			tmp =tmp<<16;
			tmp =tmp|(src[i*3+1]<<8);
			tmp = tmp|src[i*3+2];
			*(mem+j*xres+x1+i) = tmp;
		}
		j++;
	}

	(void) jpeg_finish_decompress(&cinfo);
	fclose(infile);
	munmap(src,width*depth);
	
	return 0;
}
#endif
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
