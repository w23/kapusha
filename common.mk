.SUFFIXES:
METADEPS = Makefile $(KAPUSHA_ROOT)/kapusha.mk $(KAPUSHA_ROOT)/common.mk
CXXFLAGS += $(CFLAGS) -std=c++11 -Wall -Werror -fno-exceptions -fno-rtti -I$(KAPUSHA_ROOT)
LDFLAGS += -lm

ifneq ($(RELEASE),1)
	DEBUG = 1
endif

ifeq ($(OS),Windows_NT)
	WITH_WINDOWS = 1
endif

ifneq ($(WITH_WINDOWS), 1)
ifneq ($(WITH_SDL),1)
	WITH_X11 = 1
	WITH_EGL = 0
endif

ifeq ($(WITH_RASPBERRY),1)
	WITH_SDL = 0
	WITH_X11 = 0
	WITH_EGL = 1
endif # RP
endif

ifeq ($(DEBUG),1)
	CXXFLAGS += -O0 -g -DDEBUG=1
else
	CXXFLAGS += -Os -fomit-frame-pointer
endif

ifeq ($(WITH_RASPBERRY),1)
	CXXFLAGS += -DKP_RASPBERRY=1 -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
	CXXFLAGS += -I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -lGLESv2 -lEGL -lbcm_host -L/opt/vc/lib
endif

ifeq ($(WITH_X11),1)
	WITH_HARFTYPE = 1
	LDFLAGS += -lGL -lX11 -lrt
endif

ifeq ($(WITH_SDL),1)
	WITH_HARFTYPE = 1
	CXXFLAGS += `pkg-config --cflags sdl`
	LDFLAGS += `pkg-config --libs sdl` -lGL
endif

ifeq ($(WITH_WINDOWS),1)
# TODO get rid of glew
	CFLAGS += -I$(KAPUSHA_ROOT)/3p/glew -DUNICODE -D_UNICODE -DGLEW_STATIC
	LDFLAGS += -luser32 -lopengl32 -lgdi32 -lwinmm
	LDFLAGS += -static-libgcc -static-libstdc++
endif

ifeq ($(WITH_STRIP), 1)
	LDFLAGS += -s
endif

# temp hack
WITH_HARFTYPE = 0

ifeq ($(WITH_HARFTYPE),1)
	CXXFLAGS += `pkg-config --cflags freetype2 harfbuzz`
	LDFLAGS += `pkg-config --libs freetype2 harfbuzz`
endif

%.o: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.d: %.cpp $(METADEPS)
	$(CXX) $(CXXFLAGS) -MM $< -MT $(patsubst %.cpp,%.o,$<) -MF $@
