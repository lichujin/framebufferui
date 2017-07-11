#include "SeekBar.h"
#include "screen.h"
#include "include/config.h"


SeekBar::SeekBar(int v1,int v2,int v3,int v4,int p,int m):view(v1,v2,v3,v4)
{
	progress = p;
	max = m;
}
string SeekBar::GetText(){
	return text;
}
void SeekBar::SetText(string s){
	text = s;
}
void SeekBar::SetFunc(int (*click)(void *),void *t){
	func = click;
	data = t;
}
int SeekBar::OnDraw(){
	int y;
	int per;

	y = y1+(y2-y1-SEEKBAR_HEIGHT)/2;
	screen::fill_rect(x1,y,x2,y+SEEKBAR_HEIGHT,SEEKBAR_BACK_COLOR);

	per = (x2-x1)/(max+1);

	if(progress*per <=10){
		screen::fill_rect(x1,y,x1+20,y+SEEKBAR_HEIGHT,SEEKBAR_FORE_COLOR);
	}else if(screen::x >=(x2-20)){
		screen::fill_rect(x2-20,y,x2,y+SEEKBAR_HEIGHT,SEEKBAR_FORE_COLOR);
	}else{
		screen::fill_rect(x1+progress*per-10,y,x1+progress*per+10,y+SEEKBAR_HEIGHT,SEEKBAR_FORE_COLOR);
	}
	return 0;
}
int SeekBar::OnClick(){
	int per;

	per = (x2-x1)/(max+1);
	if(screen::posinrect(x1,y1,x2,y2)){
		progress = (screen::x-x1)/per;
		OnDraw();
		if(func !=NULL)
			func((void *)this);
	}
	return progress;
}
int SeekBar::OnKeyPress(int value){
	return 0;
}
int SeekBar::OnFocus(){
	return 0;
}
int SeekBar::OnLostFocus(){
	return 0;
}

SeekBar::~SeekBar(){
}


