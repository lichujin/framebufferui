#include "Spinner.h"
#include "screen.h"
#include "include/config.h"
#include <stdio.h>

Spinner::Spinner(int v1,int v2,int v3,int v4):view(v1,v2,v3,v4)
{
	pos = -1;
}
string Spinner::GetText()
{
	return text;
}
void Spinner::SetText(string s)
{
	text = s;
}
void Spinner::SetFunc(int (*click)(void *),void *t){
	func = click;
	data = t;
}
int Spinner::OnDraw(){
	
	screen::draw_rect(x1,y1,x2,y2,BORDER_COLOR);
	screen::fill_rect(x1+1,y1+1,x2-1,y2-1,COLOR_WHITE);
	screen::fill_rect((x2-30),y1,x2,y2,SPINNER_BACK_COLOR);
	if(text.length()!=0)
		screen::draw_string(x1,y1,text,COLOR_TEXT);
}
int Spinner::OnClick(){
	int i,j,k=0;

	if(screen::posinrect(x1,y1,x2,y2)){
		if(back.empty()){
			for(i=y2;i<y2+item.size()*SPINNER_ITEM_HEIGHT;i++)
			for(j=x1;j<x2;j++)
				back.push_back(*(screen::mem+i*screen::xres+j));

			for(i=0;i<item.size();i++){
				screen::fill_rect(x1,y2+i*SPINNER_ITEM_HEIGHT,x2,y2+(i+1)*SPINNER_ITEM_HEIGHT,SPINNER_ITEM_NOSEL_COLOR);
				screen::draw_string(x1,y2+i*SPINNER_ITEM_HEIGHT,item[i],COLOR_TEXT);
			}
			
		}else{
			if(!back.empty()){
				for(i=y2;i<y2+item.size()*SPINNER_ITEM_HEIGHT;i++)
				for(j=x1;j<x2;j++)
					*(screen::mem+i*screen::xres+j) = back[k++];
				pos = -1;
				back.clear();
			}
		}
	}else if(!back.empty()&&screen::posinrect(x1,y2,x2,y2+item.size()*SPINNER_ITEM_HEIGHT)){
		i = (screen::y-y2)/SPINNER_ITEM_HEIGHT;
		text = item[i];
		OnDraw();
		if(func != NULL)
			func(this);
	}else{
		if(!back.empty()){
			for(i=y2;i<y2+item.size()*SPINNER_ITEM_HEIGHT;i++)
			for(j=x1;j<x2;j++)
				*(screen::mem+i*screen::xres+j) = back[k++];
			pos = -1;
			back.clear();
		}

	}
		return 0;
}
int Spinner::OnKeyPress(int value){
	return 0;
}
int Spinner::OnFocus(){
	if(!back.empty()&&screen::posinrect(x1,y2,x2,y2+item.size()*SPINNER_ITEM_HEIGHT)){
		if(pos>=0)
			OnLostFocus();
		pos = (screen::y-y2)/SPINNER_ITEM_HEIGHT;
		screen::fill_rect(x1,y2+pos*SPINNER_ITEM_HEIGHT,x2,y2+(pos+1)*SPINNER_ITEM_HEIGHT,SPINNER_ITEM_SEL_COLOR);
		screen::draw_string(x1,y2+pos*SPINNER_ITEM_HEIGHT,item[pos],COLOR_TEXT);
	}
	return 0;
}
int Spinner::OnLostFocus(){
	screen::fill_rect(x1,y2+pos*SPINNER_ITEM_HEIGHT,x2,y2+(pos+1)*SPINNER_ITEM_HEIGHT,SPINNER_ITEM_NOSEL_COLOR);
	screen::draw_string(x1,y2+pos*SPINNER_ITEM_HEIGHT,item[pos],COLOR_TEXT);
	return 0;
}
int Spinner::Add(string s)
{	
	int i;
	for(i=0;i<item.size();i++)
		if(item[i] == s) return i;
	item.push_back(s);
	return item.size();
}
Spinner::~Spinner(){
}

