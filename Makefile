ROOT=.
include common.mk

#! \fixme dependencies

SOURCES := \
	kapusha/core/assert.cpp \
	kapusha/core/log.cpp \
	kapusha/math/math.cpp \
	kapusha/io/StreamFile_posix.cpp \
	kapusha/io/Stream.cpp \
	kapusha/viewport/input.cpp \
	kapusha/render/Program.cpp \
	kapusha/render/Material.cpp \
	kapusha/render/Batch.cpp \
	kapusha/render/Sampler.cpp \
	kapusha/render/State.cpp \
	kapusha/render/Buffer.cpp \
	kapusha/render/OpenGL.cpp \
	kapusha/render/Context.cpp \
	kapusha/ooo/Reframe.cpp \
	kapusha/ooo/Object.cpp \
	kapusha/ooo/Camera.cpp \
	kapusha/utils/SpectatorCameraController.cpp \
	kapusha/utils/geometry.cpp \
	kapusha/utils/noise.cpp
	

ifeq ($(RPI),1)
  SOURCES += kapusha/sys/rpi/VideoCore.cpp \
  	kapusha/sys/rpi/EGL.cpp \
  	kapusha/sys/rpi/RPi.cpp \
  	kapusha/sys/rpi/Evdev.cpp
else
  SOURCES += kapusha/sys/sdl/KPSDL.cpp
endif

#MODULES=$(addprefix build/, $(patsubst %.cpp, %.o, $(SOURCES)))
MODULES=$(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all clean

libkapusha.a: $(MODULES)
	ar rcs libkapusha.a $(MODULES)

clean:
	@rm -f $(MODULES) libkapusha.a
