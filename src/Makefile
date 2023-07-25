UI = libframebuffer.a

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


SRC += Spinner.cpp
SRC += SeekBar.cpp
SRC += TextView.cpp
SRC += EditView.cpp
SRC += Button.cpp
SRC += Menu.cpp
SRC += MenuItem.cpp
SRC += screen.cpp
SRC += StatusBar.cpp
SRC += view.cpp
SRC += Window.cpp
SRC += Termios.cpp
SRC += Mouse.cpp
SRC += MessageBox.cpp


INCLUDE = ../include

include ../.config

ifeq ($(CONFIG_FREETYPE),y)
FREETYPE += -L../out/freetype/lib -lfreetype -I ../out/freetype/include/freetype2
else
FREETYPE =
endif

ifeq ($(CONFIG_PNG),y)
PNG += -L../out/libpng/lib -lpng -I ../out/libpng/include
else
PNG =
endif

ifeq ($(CONFIG_JPEG),y)
JPEG += -L../out/libjpeg/lib -ljpeg -I ../out/libjpeg/include
else
JPEG =
endif

$(UI):$(LIBS) $(SRC)
	ar rcs $@ $(LIBS)

%.o:%.cpp
	$(CC) -c $^  $(FREETYPE) $(PNG) $(JPEG) -I $(INCLUDE)

clean:
	rm *.o $(UI) -rf
