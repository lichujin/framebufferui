#ifndef _SEEK_BAR_H_
#define _SEEK_BAR_H_
#include "view.h"
class SeekBar:public view{
	public:

	int progress;
	int max;

	SeekBar(int v1,int v2,int v3,int v4,int p,int m);
	virtual string GetText();
	virtual void SetText(string s);
	virtual void SetFunc(int (*click)(void *),void *t);
	virtual int OnDraw();
	virtual int OnClick();
	virtual int OnKeyPress(int value);
	virtual int OnFocus();
	virtual int OnLostFocus();

	virtual ~SeekBar();



};


#endif
