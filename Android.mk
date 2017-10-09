LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	screen.cpp\
	view.cpp\
	Button.cpp\
	MenuItem.cpp\
	Menu.cpp\
	Window.cpp\
	Mouse.cpp\
	StatusBar.cpp\
	main.cpp


LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES

#LOCAL_CFLAGS += -Wall -Werror -Wunused -Wunreachable-code

LOCAL_C_INCLUDES += external/tinyalsa/include

#LOCAL_LDLIBS += -lpthread

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    liblog \
    libandroidfw \
    libutils \
    libbinder \
    libui \
	libmedia \
    libskia \
    libEGL \
    libGLESv1_CM \
    libgui \
    libtinyalsa

LOCAL_MODULE:= help

ifdef TARGET_32_BIT_SURFACEFLINGER
LOCAL_32_BIT_ONLY := true
endif

include $(BUILD_EXECUTABLE)
