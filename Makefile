ROOT=.
KAPUSHA_ROOT=$(ROOT)
include $(KAPUSHA_ROOT)/common.mk

SOURCES := \
	kapusha/core/assert.cpp \
	kapusha/core/log.cpp \
	kapusha/core/array.cpp \
	kapusha/core/map.cpp \
	kapusha/core/Surface.cpp \
	kapusha/math/math.cpp \
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
	kapusha/fontain/String.cpp \
	kapusha/utils/SpectatorCameraController.cpp \
	kapusha/utils/geometry.cpp \
	kapusha/utils/Atlas.cpp \
	kapusha/utils/noise.cpp

ifeq ($(KAPUSHA_RASPBERRY),1)
SOURCES += \
	kapusha/sys/rpi/VideoCore.cpp \
	kapusha/sys/rpi/EGL.cpp \
	kapusha/sys/rpi/RPi.cpp \
	kapusha/sys/rpi/Evdev.cpp
endif

ifeq ($(KP_SDL),1)
SOURCES += \
	kapusha/sys/sdl/KPSDL.cpp \
	kapusha/sys/sdl/main.cpp
endif

ifeq ($(KP_X11),1)
SOURCES += \
	kapusha/sys/x11/GLXContext.cpp \
	kapusha/sys/x11/GLXViewportController.cpp \
	kapusha/sys/x11/x11.cpp
endif

ifeq ($(KAPUSHA_WINDOWS), 1)
SOURCES += \
	3p/glew/GL/glew.cpp \
	kapusha/sys/win/Window.cpp
else
SOURCES += \
	kapusha/io/StreamFile_posix.cpp
endif

ifeq ($(KP_HARFTYPE),1)
SOURCES += \
	kapusha/fontain/harftype/Freetype.cpp \
	kapusha/fontain/harftype/Face.cpp
endif

MODULES=$(SOURCES:.cpp=.o)
DEPENDS=$(SOURCES:.cpp=.d)

.PHONY: all clean

libkapusha.a: $(MODULES)
	@rm -f libkapusha.a
	$(AR) rcs libkapusha.a $(MODULES)

depend: $(DEPENDS)

-include $(DEPENDS)

clean:
	@rm -f $(MODULES) $(DEPENDS) libkapusha.a
