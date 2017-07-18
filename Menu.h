#ifndef _MENU_H_
#define _MENU_H_
#include "MenuItem.h"
#include "view.h"
#include <vector>


using namespace std;
class Menu:public view{
	public:
	int x;
	int y;
	string text;
	int focus;
	int type;//up or down

	vector<unsigned int> back;
	vector<MenuItem> child;


	Menu(int a,int b,string t,int ty=0);

	void Add(MenuItem a);
	int OnClick(int a,int b);
	int OnDraw();

	int OnFocus();
	//int OnLostFocus();


	int Open(int v);











};
#endif
