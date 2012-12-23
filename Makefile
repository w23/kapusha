ROOT=.
include common.mk

#! \fixme dependencies

SOURCES := \
	kapusha/core/Input.cpp \
	kapusha/core/Log.cpp \
	kapusha/render/Program.cpp \
	kapusha/render/Material.cpp \
	kapusha/render/Batch.cpp \
	kapusha/render/Texture.cpp \
	kapusha/render/Buffer.cpp \
	kapusha/render/Camera.cpp \
	kapusha/render/Object.cpp \
	kapusha/render/OpenGL.cpp \
	kapusha/math/math.cpp \
	kapusha/io/StreamFile_posix.cpp \
	kapusha/io/Stream.cpp \
	kapusha/sys/SDL/KPSDL.cpp

ifeq ($(RPI),1)
SOURCES += kapusha/sys/SDL/RPi.cpp
endif

#MODULES=$(addprefix build/, $(patsubst %.cpp, %.o, $(SOURCES)))
MODULES=$(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all clean

libkapusha.a: $(MODULES)
	ar rcs libkapusha.a $(MODULES)

clean:
	@rm -rf $(MODULES) libkapusha.a
