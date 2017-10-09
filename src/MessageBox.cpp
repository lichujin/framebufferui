#include "screen.h"
#include "Termios.h"
#include "Mouse.h"
#include "MessageBox.h"
#include "config.h"

#include <unistd.h>
#include <pthread.h>



int MessageBox::type;
int MessageBox::time;
string MessageBox::title;
string MessageBox::content;
vector<unsigned int> MessageBox::back;

static void *func(void *p){
	int xb,yb,xe,ye;
	int i,j,k=0;

	string str = MessageBox::title+":"+MessageBox::content;

	MessageBox::back.clear();

	xb = (screen::xres -(CHAR_WIDTH+2)*str.length())/2;
	yb = (screen::yres - CHAR_HEIGHT)/2;
	xe = xb +(CHAR_WIDTH+2)*str.length();
	ye = yb +CHAR_HEIGHT+1;

	for(i = yb;i<=ye;i++)
	for(j = xb;j<=xe;j++)
		MessageBox::back.push_back(*(screen::mem+i*screen::xres_virtual+j));


	#ifdef CONFIG_INPUT_MOUSE
		Mouse::enable = 0;
	#endif
	for(i = 0;i<MessageBox::time;i++){
		screen::fill_roundrect(xb,yb,xe,ye,5,MESSAGEBOX_BACK_COLOR-i*100);
		screen::draw_string(xb,yb,str,COLOR_TEXT);
		screen::draw_roundrect(xb,yb,xe,ye,5,MESSAGEBOX_BORDER_COLOR);
		sleep(1);
	}
	
	#ifdef CONFIG_INPUT_MOUSE
		Mouse::enable = 1;
	#endif
	for(i = yb;i<=ye;i++)
	for(j = xb;j<=xe;j++)
		*(screen::mem+i*screen::xres_virtual+j) = MessageBox::back[k++];
	MessageBox::back.clear();
}

int MessageBox::show(string t,string c,int ti,int tt)
{
	int ret;
	pthread_t pid;

	title = t;
	content = c;
	time = ti;
	type = tt;


	ret = pthread_create(&pid,NULL,func,NULL);
	if(ret){
		printf("Init thread failed\n");
		return -1;
	}
	pthread_join(pid,NULL);
	

	return 0;
}
