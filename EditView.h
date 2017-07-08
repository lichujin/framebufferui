#ifndef _EDIT_VIEW_H_
#define _EDIT_VIEW_H_
#include "view.h"



class EditView:public view{
	public:

	int pos;
	pthread_t cursor;

	EditView(int v1,int v2,int v3,int v4);
	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~EditView();






};

#endif
