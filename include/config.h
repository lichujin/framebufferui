#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "color.h"
#include "dimen.h"

#define DEVICE		"/dev/fb0"
#define MOUSE		"/dev/input/event8"


#define INPUT_MOUSE
#define INPUT_TERMIOS







#define EVENT_MOUSE_MOVE	0x8
#define EVENT_LEFT_CLICK	0x9
#define EVENT_RIGHT_CLICK	0xa
#define EVENT_KEY_PRESS		0xb


#endif
