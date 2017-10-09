#include "MenuItem.h"
#include "screen.h"


MenuItem::MenuItem(string s):view(0,0,0,0)
{
	text = s;
	focusable = 1;
}
string MenuItem::GetText(){
	return text;
}
void MenuItem::SetText(string s){
	text = s;
}
void MenuItem::SetFunc(int (*click)(void *),void *t){
	func = click;
	data = t;
}
int MenuItem::OnDraw(){
	screen::fill_rect(x1,y1,x2,y2,MENU_BACK_COLOR);
	screen::draw_line_(x1,y1,(x2-x1),MENU_SEP_COLOR);
	screen::draw_line_(x1,y2,(x2-x1),MENU_SEP_COLOR);
	screen::draw_string(x1,y1,text,COLOR_TEXT);
	return 0;
}
int MenuItem::OnClick(){
	if(func !=NULL)
		return func(data);
	else
		return 0;
}
int MenuItem::OnKeyPress(int value){return 0;}
int MenuItem::OnFocus(){
	screen::fill_rect(x1,y1,x2,y2,MENU_FOCUS_COLOR);
	screen::draw_line_(x1,y1,(x2-x1),MENU_SEP_COLOR);
	screen::draw_line_(x1,y2,(x2-x1),MENU_SEP_COLOR);
	screen::draw_string(x1,y1,text,COLOR_TEXT);
	return 0;
}
int MenuItem::OnLostFocus(){
	screen::fill_rect(x1,y1,x2,y2,MENU_BACK_COLOR);
	screen::draw_line_(x1,y1,(x2-x1),MENU_SEP_COLOR);
	screen::draw_line_(x1,y2,(x2-x1),MENU_SEP_COLOR);
	screen::draw_string(x1,y1,text,COLOR_TEXT);
	return 0;
}
MenuItem::~MenuItem(){
}

