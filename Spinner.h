#ifndef _SPINNER_H_
#define _SPINNER_H_

#include "view.h"



#include <vector>
#include <iostream>

using namespace std;
class Spinner:public view{
	public:
	int pos;
	vector<string> item;
	vector<unsigned int> back;

	Spinner(int v1,int v2,int v3,int v4);
	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	int Add(string s);

	virtual ~Spinner();

};



#endif
