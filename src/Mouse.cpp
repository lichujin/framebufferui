#include"screen.h"
#include "Mouse.h"
#include "MessageBox.h"
#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<linux/input.h>
#include <pthread.h>

using namespace std;

int Mouse::fd = 0;
pthread_t Mouse::dev;
vector<unsigned int> Mouse::back;
int Mouse::enable = 1;
char Mouse::cursor[18][18]={{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2},
			    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2,2},
			    {1,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,2},
			    {1,0,0,0,0,0,0,1,0,0,0,0,0,1,2,2,2,2},
			    {1,0,0,0,0,0,1,2,1,0,0,0,0,0,1,2,2,2},
			    {1,0,0,0,0,1,2,2,2,1,0,0,0,0,0,1,2,2},
			    {1,0,0,0,1,2,2,2,2,2,1,0,0,0,0,0,1,2},
			    {1,0,0,1,2,2,2,2,2,2,2,1,0,0,0,1,2,2},
			    {1,0,1,2,2,2,2,2,2,2,2,2,1,0,1,2,2,2},
			    {1,1,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2},
			    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}};

int Mouse::MouseHide(){
	int i,j,k= 0;

	if(back.empty())return 0;

	for(i=0;i<18;i++)
	for(j=0;j<18;j++)
		*(screen::mem+(screen::y+i)*screen::xres_virtual+screen::x+j)=back[k++];
	back.clear();

	return 0;
}

int Mouse::MouseShow(){
	int i,j = 0;

	if(!back.empty())back.clear();

	for(i=0;i<18;i++)
	for(j=0;j<18;j++){
		back.push_back(*(screen::mem+(screen::y+i)*screen::xres_virtual+screen::x+j));
		if(cursor[i][j] == 1)
			*(screen::mem+(screen::y+i)*screen::xres_virtual+screen::x+j) = MOUSE_ICON_COLOR;
		else if(cursor[i][j] == 0)
			*(screen::mem+(screen::y+i)*screen::xres_virtual+screen::x+j) = COLOR_WHITE;
		else ;
	}

	return 0;
}

static void *func(void *p){
	struct input_event ev;

	Mouse::fd = open(CONFIG_MOUSE_DEVICE,O_RDONLY);
	if(Mouse::fd<=0){
		MessageBox::show("Error","Open input device failed",5);
		return NULL;
	}

	while(!screen::exit){
		read(Mouse::fd,&ev,sizeof(struct input_event));
		if(0 == Mouse::enable)continue;
		switch(ev.type){
			case EV_KEY:
			{
				switch(ev.code){
					case BTN_LEFT:
						if(ev.value == 0){
							screen::sendmessage(EVENT_LEFT_CLICK);
						}
						break;
					case BTN_RIGHT:
						if(ev.value == 0){
							screen::sendmessage(EVENT_RIGHT_CLICK);
						}
						break;
					defaule:
					break;
				}
			}
			break;
			case EV_REL:
			{
				Mouse::MouseHide();
				switch(ev.code){
					case REL_X:
					screen::x += ev.value;
					if(screen::x<0)
						screen::x = 0;
					else if((screen::x+20)>screen::xres)
						screen::x = screen::xres-20;
					break;
					case REL_Y:
					screen::y += ev.value;
					if(screen::y<0)
						screen::y = 0;
					else if((screen::y+20)>screen::yres)
						screen::y = screen::yres-20;
					break;
					default:
					break;
				}
				screen::sendmessage(EVENT_MOUSE_MOVE);
				Mouse::MouseShow();
			}
			break;
		}
	}



	return 0;
}
int Mouse::start(){
	int ret = 0;

	MouseShow();

	fd =open(CONFIG_MOUSE_DEVICE,O_RDONLY);
	if(fd <0){
		MessageBox::show("Error","Open Mouse device failed",5);
		return -1;
	}
	ret = pthread_create(&dev,NULL,func,NULL);
	if(ret){
		MessageBox::show("Error","Init Mouse thread failed",5);
		return -1;
	}
	
	return 0;
}
int Mouse::destroy(){
	close(fd);
	pthread_join(dev,NULL);

	return 0;
}