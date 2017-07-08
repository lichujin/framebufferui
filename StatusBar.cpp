#include "screen.h"
#include "StatusBar.h"
#include <stdio.h>


StatusBar::StatusBar():view(0,0,0,0)
{
	x1 = 0;
	y1 = screen::yres - STATUSBAR_HEIGHT;
	x2 = screen::xres-1;
	y2 = screen::yres-1;
}
void StatusBar::SetFunc(int (*click)(void *),void *t){

}
int StatusBar::OnDraw(){
	int i,x,y;

	x1 = 0;
	y1 = screen::yres - STATUSBAR_HEIGHT;
	x2 = screen::xres-1;
	y2 = screen::yres-1;

	x = x1+2;
	y = y1+4;

	screen::fill_rect(x1,y1,x2,y2,STATUS_BACK_COLOR);
	screen::draw_line_(x1,y1,x2,STATUS_BACK_COLOR+6);
	screen::draw_line_(x1,y1+1,x2,STATUS_BACK_COLOR+4);
	screen::draw_line_(x1,y1+2,x2,STATUS_BACK_COLOR+2);
	screen::draw_line_(x1,y2-2,x2,STATUS_BACK_COLOR+2);
	screen::draw_line_(x1,y2-1,x2,STATUS_BACK_COLOR+4);
	screen::draw_line_(x1,y2,x2,STATUS_BACK_COLOR+6);
	for(i=0;i<screen::child.size();i++){
		screen::draw_rect(x,y,x+STATUSBAR_ITEM_WIDTH,y+STATUSBAR_ITEM_HEIGHT,BORDER_COLOR);
		if(screen::child[i]->text.length()>6)
			screen::draw_string(x+1,y+1,screen::child[i]->text.substr(0,6),COLOR_TEXT);
		else
			screen::draw_string(x+1,y+1,screen::child[i]->text,COLOR_TEXT);
		x+=STATUSBAR_ITEM_WIDTH+2;
	}

	return 0;
}
int StatusBar::OnClick(){
	int click = screen::x/(STATUSBAR_ITEM_WIDTH+2);

	if(click<0 ||click>=screen::child.size()) return 0;

	if(click !=screen::activewin){
		if(screen::activewin !=0xFF)
			screen::child[screen::activewin]->Hide();	
		screen::child[click]->Show();
		screen::activewin = click;
	}else{
		if(screen::child[click]->back.empty())
			screen::child[click]->Show();
		else
			screen::child[click]->Hide();
	}
	return 0;
}
int StatusBar::OnKeyPress(int value){return 0;}
int StatusBar::OnFocus(){
	return 0;
}
int StatusBar::OnLostFocus(){
	return 0;
}
StatusBar::~StatusBar()
{
}

