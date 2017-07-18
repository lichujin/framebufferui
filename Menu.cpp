#include "Menu.h"
#include "screen.h"
#include "include/config.h"


Menu::Menu(int a,int b,string t,int ty):view(x,y,x,y){
	x = a;
	y = b;
	text = t;
	type = ty;
	focus = -1;
}
void Menu::Add(MenuItem a){
	int size = child.size();
	if(type ==0){//down type
		a.x1 = x;
		a.y1 = y+(size+1)*MENU_ITEM_HEIGHT;
		a.x2 = a.x1+MENU_ITEM_WIDTH;
		a.y2 = a.y1+MENU_ITEM_HEIGHT;
		child.push_back(a);
	}else{//up type
		a.x1 = x;
		a.y1 = y-(size+1)*MENU_ITEM_HEIGHT;
		a.x2 = a.x1+MENU_ITEM_WIDTH;
		a.y2 = a.y1+MENU_ITEM_HEIGHT;
		child.push_back(a);
	}
}
int Menu::OnClick(int a,int b){
	int ret = 0;
	int i;
	int end = child.size()+1;
	int click;

	if((a>=x)&&(a<=x+MENU_ITEM_WIDTH)){
		if(0 == type){//down type
			if((b>=y)&&(b<=y+MENU_ITEM_HEIGHT)){
				if(back.empty()){
					Open(1);
					for(i = 0;i<child.size();i++)
						child[i].OnDraw();
				}else{
					Open(0);
				}
			}else if((b>y+MENU_ITEM_HEIGHT)&&(b<=y+end*MENU_ITEM_HEIGHT)&&
				!back.empty()){
				click = (b-y)/MENU_ITEM_HEIGHT-1;
				child[click].OnClick();
			}
		}else if(1==type){//up type
			if((b>=y)&&(b<=y+MENU_ITEM_HEIGHT)){
				if(back.empty()){
					Open(1);
					for(i = 0;i<child.size();i++)
						child[i].OnDraw();
				}else{
					Open(0);
				}
			}else if((b>=y-(end-1)*MENU_ITEM_HEIGHT)&&(b<y)&&
				!back.empty()){
				click = (y-b)/MENU_ITEM_HEIGHT;
				child[click].OnClick();
			}

		}else return -1;
	}
	return ret;
}
int Menu::OnFocus(){
	if(back.empty())return 0;
	if(0 == type){//down type
		if(screen::posinrect(x,y+MENU_ITEM_HEIGHT,x+MENU_ITEM_WIDTH,y+(child.size()+1)*MENU_ITEM_HEIGHT))
		{
			if((screen::y-y)/MENU_ITEM_HEIGHT-1 != focus)
			{
				if(focus >=0)
					child[focus].OnLostFocus();
				//child[(screen::y-y)/MENU_ITEM_HEIGHT-1].OnFocus();
				screen::focus_view = &child[(screen::y-y)/MENU_ITEM_HEIGHT-1];
				screen::focus_view->OnFocus();
				focus = (screen::y-y)/MENU_ITEM_HEIGHT-1;
			}
		}
	}else{
		if(screen::posinrect(x,y-child.size()*MENU_ITEM_HEIGHT,x+MENU_ITEM_WIDTH,y))
		{
			if((y-screen::y)/MENU_ITEM_HEIGHT != focus)
			{
				if(focus >=0)
					child[focus].OnLostFocus();
				//child[(y-screen::y)/MENU_ITEM_HEIGHT].OnFocus();
				screen::focus_view = &child[(y-screen::y)/MENU_ITEM_HEIGHT];
				screen::focus_view->OnFocus();
				focus = (y-screen::y)/MENU_ITEM_HEIGHT;
			}
		}

	}
	return 0;
}
int Menu::OnDraw(){
	screen::fill_rect(x,y,(x+MENU_ITEM_WIDTH),(y+MENU_ITEM_HEIGHT),MENU_BACK_COLOR);
	screen::draw_string(x,y,text,COLOR_TEXT);
	return 0;
}
int Menu::Open(int v){
	int i,j,k = 0,height;
	if(0 == type){
		height = y+(child.size()+1)*MENU_ITEM_HEIGHT+1;
		if(v ==1){
			for(i = y+MENU_ITEM_HEIGHT;i<height;i++)
				for(j = x;j<x+MENU_ITEM_WIDTH;j++)
					back.push_back(*(screen::mem+i*screen::xres+j));
		}else{
			for(i = y+MENU_ITEM_HEIGHT;i<height;i++)
				for(j = x;j<x+MENU_ITEM_WIDTH;j++)
					*(screen::mem+i*screen::xres+j) =back[k++];
			back.clear();
			focus = -1;
		}
	}else if(1 == type){
		height = y-child.size()*MENU_ITEM_HEIGHT-1;
		if(v ==1){
			for(i = height;i<y;i++)
				for(j = x;j<x+MENU_ITEM_WIDTH;j++)
					back.push_back(*(screen::mem+i*screen::xres+j));
		}else{
			for(i = height;i<y;i++)
				for(j = x;j<x+MENU_ITEM_WIDTH;j++)
					*(screen::mem+i*screen::xres+j) =back[k++];
			back.clear();
			focus = -1;
		}

	}else return -1;
	return 0;
} 

