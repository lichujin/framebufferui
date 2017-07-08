#ifndef _MOUSE_H_
#define _MOUSE_H_

class Mouse{
	public:
	static int fd;
	static pthread_t dev;
	static vector<unsigned int> back;
	static int enable;
	static char cursor[18][18];

	static int start();
	static int MouseHide();
	static int MouseShow();
};
#endif
