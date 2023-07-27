#include"screen.h"
#include "Termios.h"
#include "Mouse.h"
#include "MessageBox.h"
#include "config.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

using namespace std;

int Termios::enable = 1;
pthread_t Termios::dev;
struct termios Termios::conf;
struct termios Termios::save;

static void *func(void *p){
	int s;
	int s1;
	int s2;

	while(!screen::exit){
		s = getchar();
		if ((s >= 32) && (s <= 127)) { // ASCII 打印字符
			screen::sendmessage(EVENT_KEY_PRESS,s);
		} else if (s == 27) { // 上下左右控制键
#ifdef CONFIG_INPUT_MOUSE
			Mouse::MouseHide();
#endif
			s1 = getchar();
			s2 = getchar();
			if (s2 == 65) { // up
				screen::y -= 10;
				if(screen::y<0)
					screen::y = 0;
			} else if (s2 == 66) { // down
				screen::y += 10;
				if((screen::y+20)>screen::yres)
					screen::y = screen::yres-20;
			} else if (s2 == 67) { // right
				screen::x += 10;
				if((screen::x+20)>screen::xres)
					screen::x = screen::xres-20;
			} else if (s2 == 68) { // left
				screen::x -= 10;
				if(screen::x<0)
					screen::x = 0;
			} else {

			}
			screen::sendmessage(EVENT_MOUSE_MOVE);
#ifdef CONFIG_INPUT_MOUSE
			Mouse::MouseShow();
#endif
		} else if (s == 13) {
#ifdef CONFIG_INPUT_MOUSE
			Mouse::MouseHide();
#endif
			screen::sendmessage(EVENT_LEFT_CLICK);
#ifdef CONFIG_INPUT_MOUSE
			Mouse::MouseShow();
#endif
		} else {
			printf("I don't know what happening!\n");
			screen::sendmessage(EVENT_RIGHT_CLICK);
		}
	}

	tcsetattr(0,TCSANOW,&Termios::save);
	return 0;
}
int Termios::start(){
	int ret = 0;

	tcgetattr(0,&conf);
	tcgetattr(0,&save);
	cfmakeraw(&conf);
	tcsetattr(0,TCSANOW,&conf);

	ret = pthread_create(&dev,NULL,func,NULL);
	if(ret){
		MessageBox::show("Error","Init Termios thread failed",5);
		return -1;
	}
	
	return 0;
}
int Termios::destroy(){
	tcsetattr(0,TCSANOW,&save);
	pthread_cancel(dev);
}
