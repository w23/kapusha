ROOT=.
include $(ROOT)/common.mk

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
	kapusha/render/Framebuffer.cpp \
	kapusha/ooo/Reframe.cpp \
	kapusha/ooo/Object.cpp \
	kapusha/ooo/Camera.cpp \
	kapusha/ooo/Node.cpp \
	kapusha/utils/SpectatorCameraController.cpp \
	kapusha/utils/geometry.cpp \
	kapusha/utils/noise.cpp
	

ifeq ($(KP_RASPBERRY),1)
SOURCES += \
  kapusha/sys/rpi/VideoCore.cpp \
  kapusha/sys/rpi/EGL.cpp \
  kapusha/sys/rpi/RPi.cpp \
  kapusha/sys/rpi/Evdev.cpp
endif

ifeq ($(KP_SDL),1)
SOURCES += \
  kapusha/sys/sdl/KPSDL.cpp
endif

ifeq ($(KP_X11),1)
SOURCES += \
  kapusha/sys/x11/GLXContext.cpp \
  kapusha/sys/x11/GLXViewportController.cpp \
  kapusha/sys/x11/x11.cpp
endif

#MODULES=$(addprefix build/, $(patsubst %.cpp, %.o, $(SOURCES)))
MODULES=$(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all clean

libkapusha.a: $(MODULES)
	ar rcs libkapusha.a $(MODULES)

clean:
	@rm -f $(MODULES) libkapusha.a
