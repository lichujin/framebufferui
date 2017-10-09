#include "screen.h"
#include "EditView.h"
#include "MessageBox.h"
#include "config.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>


static void *funcursor(void *p){
	int x,y,oldx,i=0;

	EditView *t = (EditView *)p;

	while(1){
		x = t->x1+t->pos*CHAR_WIDTH+2;
		y = t->y1+(t->y2-t->y1-CHAR_HEIGHT)/2;
		if(0==i){
			screen::draw_linel(x,y,CHAR_HEIGHT,COLOR_WHITE);
			i = 1;
		}else{
			screen::draw_linel(x,y,CHAR_HEIGHT,COLOR_BLACK);
			i = 0;
		}
		sleep(1);
		oldx = x;
		screen::draw_linel(oldx,y,CHAR_HEIGHT,COLOR_WHITE);
		pthread_testcancel();
	}
}
EditView::EditView(int v1,int v2,int v3,int v4):view(v1,v2,v3,v4)
{
	cursor = 0;
	focusable = 1;
	pos = 0;
}
string EditView::GetText()
{
	return text;
}
void EditView::SetText(string s)
{
	text = s;
}
void EditView::SetFunc(int (*click)(void *),void *t)
{
	func = click;
	data = t;
}
int EditView::OnDraw()
{
	int x,y;
	x = x1+2;
	y = y1+(y2-y1-CHAR_HEIGHT)/2;
	screen::draw_roundrect(x1,y1,x2,y2,5,BORDER_COLOR);
	screen::draw_string(x,y,text,COLOR_TEXT);
	return 0;
}
int EditView::OnClick()
{
	if(screen::posinrect(x1,y1,x2,y2)){
		if(text.empty()) pos = 0;
		else{
			if((screen::x-x1)/CHAR_WIDTH>=text.length())
				pos = text.length();
			else
				pos = (screen::x-x1)/CHAR_WIDTH;
		}
		OnFocus();
	}
	return 0;
}
int EditView::OnKeyPress(int value)
{
	if(screen::focus_view != this) return 0;

	//OnLostFocus();
	if(pos == text.length())
		text.append(1,(char)value);
	else
		text.insert(pos,1,(char)value);
	pos++;
	OnDraw();
	OnFocus();
	return 0;
}
int EditView::OnFocus(){
	int ret;
	
	if(screen::posinrect(x1,y1,x2,y2)){	
		screen::focus_view = this;
		if(cursor == 0){
			ret = pthread_create(&cursor,NULL,funcursor,(void *)this);
			if(ret){
				MessageBox::show("Error","Init EditView cursor thread failed",5);
				return -1;
			}
		}
	}else
		OnLostFocus();	
	return 0;	
}
int EditView::OnLostFocus()
{
	if(cursor!=0){
		pthread_cancel(cursor);
		cursor = 0;
	}
	return 0;
}
EditView::~EditView()
{
}

