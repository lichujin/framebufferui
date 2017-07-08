#include "view.h"
#include "screen.h"

view::view(int v1,int v2,int v3,int v4)
{
	x1=v1;y1=v2;x2=v3;y2=v4;
	func = NULL;
}

string view::GetText(){return text;}
void view::SetText(string s){text = s;}
void view::SetFunc(int (*click)(void *),void *t){func = click;data = t;}
int view::OnDraw(){
	return 0;
};
int view::OnClick(){
	if(!screen::posinrect(x1,y1,x2,y2))
		return 0;
	else{
		if(func !=NULL)
			return func(data);
		else
			return 0;
	}
};
int view::OnKeyPress(int value){return 0;}
int view::OnFocus(){return 0;}
int view::OnLostFocus(){return 0;}

view::~view(){};


