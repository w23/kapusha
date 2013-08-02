.SUFFIXES:
METADEPS=Makefile $(KAPUSHA_ROOT)/common.mk
CXXFLAGS += $(CFLAGS) -std=c++11 -Wall -Werror -fno-exceptions -fno-rtti -I$(KAPUSHA_ROOT)
LDFLAGS += -lm

ifneq ($(RELEASE),1)
	DEBUG = 1
endif

ifeq ($(OS),Windows_NT)
	KAPUSHA_WINDOWS = 1
endif

ifneq ($(KAPUSHA_WINDOWS), 1)
ifneq ($(KP_SDL),1)
	KP_X11 = 1
	KP_EGL = 0
endif

ifeq ($(KAPUSHA_RASPBERRY),1)
	KP_SDL = 0
	KP_X11 = 0
	KP_EGL = 1
endif # RP
endif

ifeq ($(DEBUG),1)
	CXXFLAGS += -O0 -g -DDEBUG=1
else
	CXXFLAGS += -Os -fomit-frame-pointer
endif

ifeq ($(KAPUSHA_RASPBERRY),1)
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

ifeq ($(KAPUSHA_WINDOWS),1)
# TODO get rid of glew
	CFLAGS += -I$(KAPUSHA_ROOT)/3p/glew -DUNICODE -D_UNICODE -DGLEW_STATIC
	LDFLAGS += -luser32 -lopengl32 -lgdi32 -static-libgcc -static-libstdc++
endif

ifeq ($(KP_STRIP), 1)
	LDFLAGS += -s
endif

# temp hack
KP_HARFTYPE = 0

ifeq ($(KP_HARFTYPE),1)
	CXXFLAGS += `pkg-config --cflags freetype2 harfbuzz`
	LDFLAGS += `pkg-config --libs freetype2 harfbuzz`
endif

%.o: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -MM $< -MT $(patsubst %.cpp,%.o,$<) -MF $@
