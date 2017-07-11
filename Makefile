
CC = g++
TAR = main

LIBS += Spinner.o
LIBS += SeekBar.o
LIBS += TextView.o
LIBS += EditView.o
LIBS += Button.o
LIBS += Menu.o
LIBS += MenuItem.o
LIBS += screen.o
LIBS += StatusBar.o
LIBS += view.o
LIBS += Window.o
LIBS += Termios.o
LIBS += Mouse.o
LIBS += MessageBox.o
LIBS += main.o


$(TAR):$(LIBS)
	$(CC) -o $@ $^ -lpthread -lm

Spinner.o:Spinner.cpp
	$(CC) -c $^
SeekBar.o:SeekBar.cpp
	$(CC) -c $^
TextView.o:TextView.cpp
	$(CC) -c $^
EditView.o:EditView.cpp
	$(CC) -c $^
Button.o:Button.cpp
	$(CC) -c $^
Menu.o:Menu.cpp
	$(CC) -c $^
MenuItem.o:MenuItem.cpp
	$(CC) -c $^
screen.o:screen.cpp
	$(CC) -c $^
StatusBar.o:StatusBar.cpp
	$(CC) -c $^
view.o:view.cpp
	$(CC) -c $^
Window.o:Window.cpp
	$(CC) -c $^
Termios.o:Termios.cpp
	$(CC) -c $^
Mouse.o:Mouse.cpp
	$(CC) -c $^
MessageBox.o:MessageBox.cpp
	$(CC) -c $^
main.o:main.cpp
	$(CC) -c $^

clean:
	rm *.o -rf
