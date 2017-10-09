#include "TextView.h"
#include "screen.h"
#include "config.h"


TextView::TextView(int v1,int v2,int v3,int v4,string s):view(v1,v2,v3,v4)
{
	text = s;
}
string TextView::GetText(){
	return text;
}
void TextView::SetText(string s){
	text = s;
}
void TextView::SetFunc(int (*click)(void *),void *t){
}
int TextView::OnDraw(){
	int x,y;

	if((y2-y1)<CHAR_HEIGHT) return -1;

	x = x1+3;
	y = y1+(y2-y1-CHAR_HEIGHT)/2;

	screen::draw_string(x,y,text,COLOR_TEXT);

	return 0;
}
int TextView::OnClick(){
	return 0;
}
int TextView::OnKeyPress(int value){
	return 0;
}
int TextView::OnFocus(){
	return 0;
}
int TextView::OnLostFocus(){
	return 0;
}
TextView::~TextView(){
}

