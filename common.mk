.SUFFIXES:
METADEPS=Makefile $(KAPUSHA_ROOT)/common.mk
CXXFLAGS += $(CFLAGS) -std=c++11 -Wall -Werror -fno-exceptions -fno-rtti -I$(KAPUSHA_ROOT)
LDFLAGS += -lm

ifneq ($(RELEASE),1)
	DEBUG = 1
endif

ifneq ($(KP_SDL),1)
	KP_X11 = 1
	KP_EGL = 0
endif

ifeq ($(RPI),1)
	KP_RASPBERRY = 1
	KP_SDL = 0
	KP_X11 = 0
	KP_EGL = 1
endif

ifeq ($(DEBUG),1)
	CXXFLAGS += -O0 -g -DDEBUG=1
else
	CXXFLAGS += -Os -fomit-frame-pointer
endif

ifeq ($(KP_RASPBERRY),1)
	CXXFLAGS += -DKP_RASPBERRY=1 -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
	CXXFLAGS += -I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -lGLESv2 -lEGL -lbcm_host -L/opt/vc/lib
endif

ifeq ($(KP_X11),1)
	KP_HARFTYPE = 1
	LDFLAGS += -lGL -lX11 -lrt
endif

ifeq ($(KP_SDL),1)
	KP_HARFTYPE = 1
	CXXFLAGS += `pkg-config --cflags sdl`
	LDFLAGS += `pkg-config --libs sdl` -lGL
endif

KP_HARFTYPE=0

ifeq ($(KP_HARFTYPE),1)
	CXXFLAGS += `pkg-config --cflags freetype2 harfbuzz`
	LDFLAGS += `pkg-config --libs freetype2 harfbuzz`
endif

%.o: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -MM $< -MT $(patsubst %.cpp,%.o,$<) -MF $@
