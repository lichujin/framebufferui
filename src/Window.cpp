#include "Window.h"
#include "screen.h"
#include "Termios.h"
#include "Mouse.h"
#include "MessageBox.h"
#include <stdio.h>

Window::Window():view(0,0,0,0)
{
	focusable = 1;
}
Window::Window(int a,int b,int c,int d,string str,char oa):view(a,b,c,d)
{
	onlyarea = oa;
	text = str;
	focusable = 1;
}

string Window::GetText(){
	return text;
}
void Window::SetText(string s){
	text = s;
}
void Window::SetFunc(int (*click)(void *),void *t){
	func = click;
	data = t;
}
int Window::OnDraw(){
	int ret = 0;
	int i = 0;
	int y = y1+WINDOW_TITLE_HEIGHT;

	if(0 == onlyarea){	
		screen::draw_rect(x1,y1,x2,y2,WINDOW_BORDER_COLOR);
		screen::fill_rect(x1+1,y1+1,x2-1,y-1,WINDOW_TITLE_BACK_COLOR);
		screen::draw_string(x1+5,y1+5,text,COLOR_TEXT);
		screen::draw_line_(x1,y,(x2-x1),WINDOW_BORDER_COLOR);
		screen::fill_rect(x1+1,y+1,x2-1,y2-1,WINDOW_AREA_BACK_COLOR);
		
		screen::draw_exit_icon(x2-20,y1+WINDOW_TITLE_HEIGHT/2,COLOR_BLACK);
		screen::draw_min_icon(x2-55,y1+WINDOW_TITLE_HEIGHT/2,COLOR_BLACK);
	}

	for(i=0;i<child.size();i++){
		child[i]->OnDraw();
	}
	return ret;
}
int Window::OnClick(){
	int i;
	if(screen::posinrect(x2-25,y1,x2-15,y1+WINDOW_TITLE_HEIGHT))
	{
		Hide();
		screen::activewin = 0xFF;
		screen::del(this);
	}
	else if(screen::posinrect(x2-55,y1,x2-45,y1+WINDOW_TITLE_HEIGHT))
	{
		Hide();
		screen::activewin = 0xFF;
	}else{
		for(i=0;i<child.size();i++)
			child[i]->OnClick();
	}
	return 0;
}
int Window::OnKeyPress(int value){return 0;}
int Window::OnFocus(){
	int i;
	if(screen::posinrect(x1,y1,x2,y2)){
		for(i=0;i<child.size();i++)
			child[i]->OnFocus();
	}else
		OnLostFocus();
	return 0;
}
int Window::OnLostFocus(){
	int i;

	for(i=0;i<child.size();i++)
		child[i]->OnFocus();
	return 0;
}
int Window::AddMenu(Menu *m){
	int i;
	if(m->x>=0&&m->x+MENU_ITEM_WIDTH<(x2-x1)&&m->y>=0&&m->y+MENU_ITEM_HEIGHT<=(y2-y1)){
		m->x += x1;
		m->y += y1;
		menu.push_back(m);
		child.push_back(m);
		if(!m->child.empty()){
			if(m->type == 0){
				for(i=0;i<m->child.size();i++){
					m->child[i]->x1 = m->x;
					m->child[i]->y1 = m->y+(i+1)*MENU_ITEM_HEIGHT;
					m->child[i]->x2 = m->child[i]->x1 + MENU_ITEM_WIDTH;
					m->child[i]->y2 = m->child[i]->y1 + MENU_ITEM_HEIGHT;
				}
			}else{
				for(i=0;i<m->child.size();i++){
					m->child[i]->x1 = m->x;
					m->child[i]->y1 = m->y-(i+1)*MENU_ITEM_HEIGHT;
					m->child[i]->x2 = m->child[i]->x1 + MENU_ITEM_WIDTH;
					m->child[i]->y2 = m->child[i]->y1 + MENU_ITEM_HEIGHT;
				}
			}
		}
	}else
		return -1;
	return 0;
}
int Window::AddChild(view *v){
	if(NULL == v->parent){
		if(onlyarea ==0){
			v->x1 += x1;
			v->y1 += y1+WINDOW_TITLE_HEIGHT;
			v->x2 += x1;
			v->y2 += y1+WINDOW_TITLE_HEIGHT;
		}else{
			v->x1 += x1;
			v->y1 += y1;
			v->x2 += x1;
			v->y2 += y1;
		}
		if(v->x1>x1&&v->x2<x2&&v->y1>y1&&v->y2<y2){
			child.push_back(v);
			v->parent = this;
		}else
			return -1;
	}
	return 0;
}
int Window::Show(){
	int i,j;

	if(!back.empty())back.clear();

	for(i = y1;i<y2+1;i++)
	for(j = x1;j<x2+1;j++)
		back.push_back(*(screen::mem+i*screen::xres_virtual+j));
	OnDraw();
	return 0;
}
int Window::Hide(){
	int i,j,k = 0;

	for(i = 0;i<child.size();i++)
		child[i]->OnLostFocus();

	for(i = y1;i<y2+1;i++)
	for(j = x1;j<x2+1;j++)
		*(screen::mem+i*screen::xres_virtual+j) = back[k++];
	back.clear();
	return 0;
}

Window::~Window(){
}

