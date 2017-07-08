#ifndef _TERMIOS_
#define _TERMIOS_
#include <termios.h>
class Termios{
	public:
	static int enable;
	static pthread_t dev;
	static struct termios conf;
	static struct termios save;

	static int start();
};
#endif
