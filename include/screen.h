#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "config.h"
#include "view.h"
#include "Window.h"
#include "StatusBar.h"

#ifdef CONFIG_FREETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#ifdef CONFIG_PNG
#include <png.h>
#endif

#ifdef CONFIG_JPEG
#include "jpeglib.h"
#endif

#include <setjmp.h>
#include <iostream>
#include <vector>


#ifdef CONFIG_JPEG
struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;
#endif


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
	#ifdef CONFIG_FREETYPE
	static FT_Library ftLibrary;
	static FT_Face ftFace;
	#endif
	#ifdef CONFIG_PNG
	static png_structp png_ptr;
	static png_infop info_ptr;
	#endif
	#ifdef CONFIG_JPEG
	static struct jpeg_decompress_struct cinfo;
  	static struct my_error_mgr jerr;
	#endif


	static int init();
	static int de_init();

	static int draw_pixel(int x1,int y1,unsigned int color);
	static int draw_line_(int x1,int y1,int len,unsigned int color);
	static int draw_linel(int x1,int y1,int len,unsigned int color);
	static int draw_rect(int x1,int y1,int x2,int y2,unsigned int color);
	static int draw_roundrect(int x1,int y1,int x2,int y2,int r,unsigned int color);
	static int fill_rect(int x1,int y1,int x2,int y2,unsigned int color);
	static int fill_roundrect(int x1,int y1,int x2,int y2,int r,unsigned int color);
	static int draw_circle(int x1,int y1,int r,unsigned int color);
	static int fill_circle(int x1,int y1,int r,unsigned int color);
	static int draw_arc(int x1,int y1,int r,int start,int end,unsigned int color);
	#ifdef CONFIG_FREETYPE
	static int draw_char(int x1,int y1,unsigned int uiCharCode,unsigned int color);
	#else
	static int draw_char(int x1,int y1,const unsigned char a,unsigned int color);
	#endif
	static int draw_string(int x1,int y1,string s,unsigned int color);

	static int draw_exit_icon(int x1,int y1,unsigned int color);
	static int draw_min_icon(int x1,int y1,unsigned int color);
	static int draw_image(int x1,int y1,string file,int alpha);
	#ifdef CONFIG_PNG
	static int draw_png(int x1,int y1,string file);
	#endif
	#ifdef CONFIG_JPEG
	static int draw_jpeg(int x1,int y1,string file);
	#endif

	static int utf8_ucde(unsigned char *tmp);
	static int posinrect(int x1,int y1,int x2,int y2);

	static int add(Window *v);
	static int del(Window *v);

	static int sendmessage(int type,int value=0);
};





#endif
