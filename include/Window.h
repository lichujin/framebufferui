#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "view.h"
#include "Menu.h"
#include "config.h"


#include <vector>
#include <iostream>

using namespace std;
class Window:public view{
	public:

	vector<view *> child;
	vector<Menu *> menu;
	vector<unsigned int> back;
	char onlyarea;

	Window();
	Window(int a,int b,int c,int d,string str,char oa = 0);

	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	int AddMenu(Menu *m);
	int AddChild(view *v);

	int Show();
	int Hide();


	virtual ~Window();
};

#endif
