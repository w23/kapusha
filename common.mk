CXX=g++
LD=g++
CXXFLAGS=-Wall -Werror -fno-exceptions -fno-rtti `pkg-config --cflags sdl` -g -DDEBUG=1 -I$(ROOT)
LDFLAGS=`pkg-config --libs sdl` -lGL -lm

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
