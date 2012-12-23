CXX=g++
LD=g++
CXXFLAGS=-Wall -Werror -fno-exceptions -fno-rtti `pkg-config --cflags sdl` -I$(ROOT)
LDFLAGS=`pkg-config --libs sdl` -lm

ifeq ($(DEBUG),1)
CXXFLAGS += -g -DDEBUG=1
else
CXXFLAGS += -Os -fomit-frame-pointer
endif

ifeq ($(RPI),1)
CXXFLAGS += -DKAPUSHA_RPI=1 -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
LDFLAGS += -lGLESv2 -lEGL -lbcm_host -L/opt/vc/lib
else
LDFLAGS += -lGL
endif

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
