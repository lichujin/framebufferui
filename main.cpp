#include "screen.h"
#include "Button.h"
#include "EditView.h"
#include "Menu.h"
#include "Window.h"
#include "Mouse.h"
#include "Termios.h"
#include "MessageBox.h"




int func_open(void *p)
{
	MessageBox::show("Info","Open Item Click",5);
	return 0;
}

int func_save(void *p)
{
	MessageBox::show("Info","Save Item Click",5);
	return 0;
}

int func_button(void *p)
{
	screen::exit = 1;
	return 0;
}
int main()
{

	screen::init();


#if 1
	Window *wina = new Window(100,100,1200,700,"Hello");
	Window *winb = new Window(100,100,800,700,"World");
	Window *winc = new Window(0,0,1366,700,"Computer");
	screen::add(wina);
	screen::add(winb);
	screen::add(winc);

	//Button *b = new Button(100,300,300,500);
	//b->SetText("EXIT");
	//b->SetFunc(func_button,NULL);

	EditView *b = new EditView(150,300,400,400);

	wina->AddChild(b);


	MenuItem d;
	d.SetText("Open");
	d.SetFunc(func_open,NULL);
	MenuItem c;
	c.SetText("Save");
	c.SetFunc(func_save,NULL);

	Menu e(600,300,"File");
	e.Add(d);
	e.Add(c);

	wina->AddMenu(e);

	//screen::draw_image(0,0,"./my.bmp",127);
#endif



	screen::de_init();

	return 0;
}

