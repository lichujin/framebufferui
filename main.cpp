#include <screen.h>
#include <Button.h>
#include <EditView.h>
#include <Menu.h>
#include <Window.h>
#include <Mouse.h>
#include <Termios.h>
#include <TextView.h>
#include <SeekBar.h>
#include <Spinner.h>
#include <MessageBox.h>




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


	TextView *txt_username = new TextView(100,150,350,200,"UserName:");
	EditView *username = new EditView(380,150,880,200);
	TextView *txt_passd = new TextView(100,220,350,270,"PassWord:");
	EditView *passwd = new EditView(380,220,880,270);
	Button *login = new Button(150,290,300,340,"Login");
	Button *reset = new Button(440,290,590,340,"Reset");

	TextView *txt_seek = new TextView(100,600,300,650,"Progress");
	SeekBar *seek = new SeekBar(320,600,620,650,50,100);

	Spinner *country = new Spinner(900,150,1300,200);
	country->Add("Chinese");
	country->Add("USA");
	country->Add("Britain");
	country->Add("Russia");
	country->Add("France");
	country->Add("Germany");
	country->Add("Japan");
	country->Add("Australia");

	Menu *mfile = new Menu(900,300,"File");
	MenuItem *mnew = new MenuItem("New");
	MenuItem *mopen = new MenuItem("Open");
	MenuItem *msave = new MenuItem("Save");
	MenuItem *mexit = new MenuItem("Exit");
	mfile->Add(mnew);
	mfile->Add(mopen);
	mfile->Add(msave);
	mfile->Add(mexit);

	winc->AddChild(txt_username);
	winc->AddChild(username);
	winc->AddChild(txt_passd);
	winc->AddChild(passwd);
	winc->AddChild(login);
	winc->AddChild(reset);

	winc->AddChild(txt_seek);
	winc->AddChild(seek);

	winc->AddChild(country);
	winc->AddMenu(mfile);


#endif



	screen::de_init();

	return 0;
}

