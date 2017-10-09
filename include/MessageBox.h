#ifndef _MESSAGEBOX_H_
#define _MESSAGEBOX_H_

#include <iostream>

using namespace std;


class MessageBox{
	public:
	static int type;
	static int time;
	static string title;
	static string content;
	static vector<unsigned int> back;

	static int show(string t,string c,int ti,int tt=0);

};
#endif
