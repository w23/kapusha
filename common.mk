CXX=g++
LD=g++
CXXFLAGS=-Wall -Werror -fno-exceptions -fno-rtti `pkg-config --cflags sdl` -I$(ROOT)
LDFLAGS=`pkg-config --libs sdl` -lGL -lm

ifeq ($(DEBUG),1)
CXXFLAGS += -g -DDEBUG=1
else
CXXFLAGS += -Os -fomit-frame-pointer
endif

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
