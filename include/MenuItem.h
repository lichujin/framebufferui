#ifndef _MENU_ITEM_H_
#define _MENU_ITEM_H_


#include "view.h"
#include "config.h"


class MenuItem:public view{
	public:

	MenuItem(string s);
	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~MenuItem();
};

#endif
