#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "include/config.h"
#include "view.h"
#include "Window.h"
#include "StatusBar.h"

#include <iostream>
#include <vector>

using namespace std;

class screen{

	public:
	static int x;
	static int y;
	static int exit;
	static int xres;
	static int yres;
	static int xres_virtual;
	static int yres_virtual;
	static unsigned int *mem;
	static int fd;
	static vector<Window *> child;
	static StatusBar statusbar;
	static int activewin;
	static view* focus_view;


	static int init();
	static int de_init();

	static int draw_pixel(int x1,int y1,unsigned int color);
	static int draw_line_(int x1,int y1,int len,unsigned int color);
	static int draw_linel(int x1,int y1,int len,unsigned int color);
	static int draw_rect(int x1,int y1,int x2,int y2,unsigned int color);
	static int fill_rect(int x1,int y1,int x2,int y2,unsigned int color);

	static int draw_char(int x1,int y1,const unsigned char a,unsigned int color);
	static int draw_string(int x1,int y1,string s,unsigned int color);

	static int draw_exit_icon(int x1,int y1,unsigned int color);
	static int draw_min_icon(int x1,int y1,unsigned int color);
	static int draw_image(int x1,int y1,string file,int alpha);

	static int utf8_ucde(unsigned char *tmp);
	static int posinrect(int x1,int y1,int x2,int y2);

	static int add(Window *v);
	static int del(Window *v);

	static int sendmessage(int type,int value=0);
};





#endif
