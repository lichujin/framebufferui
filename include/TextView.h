#ifndef _TEXT_VIEW_H_
#define _TEXT_VIEW_H_
#include "view.h"

class TextView:public view{
	public:
	TextView(int v1,int v2,int v3,int v4,string s);
	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~TextView();

};

#endif
