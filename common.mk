CXX=g++
LD=g++
CXXFLAGS=-Wall -Werror -fno-exceptions -fno-rtti `pkg-config --cflags sdl`
LDFLAGS=`pkg-config --libs sdl`

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
