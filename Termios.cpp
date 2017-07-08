#include"screen.h"
#include "Termios.h"
#include "MessageBox.h"
#include "include/config.h"

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

	while(!screen::exit){
		s = getchar();
		screen::sendmessage(EVENT_KEY_PRESS,s);
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
