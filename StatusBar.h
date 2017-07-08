#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include "view.h"
#include "include/config.h"
#include <vector>


using namespace std;

class StatusBar:public view{
	public:

	StatusBar();
	//virtual string GetText();
	//virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~StatusBar();
};




#endif

