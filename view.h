#ifndef _VIEW_H_
#define _VIEW_H_
#include <stdio.h>

#include <iostream>

using namespace std;

class view{

	public:
	string text;
	int x1,y1,x2,y2;

	void *data;
	int (*func)(void *p);

	view(int v1,int v2,int v3,int v4);

	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~view();









};



#endif
