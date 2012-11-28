ROOT=../../..
include $(ROOT)/common.mk

SOURCES := \
	../fsquad.cpp \
	../flyby_static_geometry.cpp

MODULES=$(patsubst %.cpp, %.o, $(SOURCES))

samples_SDL: $(MODULES) main.o ../../../libkapusha.a
	$(CC) $(MODULES) main.o -lkapusha $(LDFLAGS) -L$(ROOT) -lstdc++ -o samples_SDL
