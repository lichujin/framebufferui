   

#SRC = StatusBar.cpp Mouse.cpp screen.cpp view.cpp Button.cpp MenuItem.cpp Menu.cpp Window.cpp main.cpp
#TAR = main
#$(TAR):$(SRC)
#	g++ $(SRC) -o $(TAR) -lpthread

main:SeekBar.o TextView.o EditView.o Button.o Menu.o MenuItem.o screen.o StatusBar.o view.o Window.o Termios.o Mouse.o MessageBox.o main.o
	g++ -o $@ $^ -lpthread

.c.o:
	g++ -c $<

clean:
	rm *.o -rf
