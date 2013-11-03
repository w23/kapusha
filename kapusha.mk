KAPUSHA_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))
include $(KAPUSHA_ROOT)/common.mk

KAPUSHA_SRC = $(KAPUSHA_ROOT)/kapusha

SOURCES += \
	$(KAPUSHA_SRC)/core/assert.cpp \
	$(KAPUSHA_SRC)/core/log.cpp \
	$(KAPUSHA_SRC)/core/array.cpp \
	$(KAPUSHA_SRC)/core/map.cpp \
	$(KAPUSHA_SRC)/core/Surface.cpp \
	$(KAPUSHA_SRC)/math/math.cpp \
	$(KAPUSHA_SRC)/io/Stream.cpp \
	$(KAPUSHA_SRC)/io/Socket_nix.cpp \
	$(KAPUSHA_SRC)/viewport/input.cpp \
	$(KAPUSHA_SRC)/render/Program.cpp \
	$(KAPUSHA_SRC)/render/Material.cpp \
	$(KAPUSHA_SRC)/render/Batch.cpp \
	$(KAPUSHA_SRC)/render/Sampler.cpp \
	$(KAPUSHA_SRC)/render/State.cpp \
	$(KAPUSHA_SRC)/render/Buffer.cpp \
	$(KAPUSHA_SRC)/render/OpenGL.cpp \
	$(KAPUSHA_SRC)/render/Context.cpp \
	$(KAPUSHA_SRC)/render/Framebuffer.cpp \
	$(KAPUSHA_SRC)/ooo/Reframe.cpp \
	$(KAPUSHA_SRC)/ooo/Object.cpp \
	$(KAPUSHA_SRC)/ooo/Camera.cpp \
	$(KAPUSHA_SRC)/ooo/Node.cpp \
	$(KAPUSHA_SRC)/fontain/String.cpp \
	$(KAPUSHA_SRC)/utils/SpectatorCameraController.cpp \
	$(KAPUSHA_SRC)/utils/geometry.cpp \
	$(KAPUSHA_SRC)/utils/Atlas.cpp \
	$(KAPUSHA_SRC)/utils/noise.cpp

ifeq ($(WITH_RASPBERRY),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/rpi/VideoCore.cpp \
	$(KAPUSHA_SRC)/sys/rpi/EGL.cpp \
	$(KAPUSHA_SRC)/sys/rpi/RPi.cpp \
	$(KAPUSHA_SRC)/sys/rpi/Evdev.cpp
endif

ifeq ($(WITH_SDL),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/sdl/KPSDL.cpp \
	$(KAPUSHA_SRC)/sys/sdl/main.cpp
endif

ifeq ($(WITH_X11),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/x11/GLXContext.cpp \
	$(KAPUSHA_SRC)/sys/x11/GLXViewportController.cpp \
	$(KAPUSHA_SRC)/sys/x11/main.cpp
endif

ifeq ($(WITH_WINDOWS), 1)
SOURCES += \
	3p/glew/GL/glew.cpp \
	$(KAPUSHA_SRC)/sys/win/Window.cpp
else
SOURCES += \
	$(KAPUSHA_SRC)/io/StreamFile_posix.cpp
endif

ifeq ($(WITH_HARFTYPE),1)
SOURCES += \
	$(KAPUSHA_SRC)/fontain/harftype/Freetype.cpp \
	$(KAPUSHA_SRC)/fontain/harftype/Face.cpp
endif

MODULES=$(SOURCES:.cpp=.o)
DEPENDS=$(SOURCES:.cpp=.d)

-include $(DEPENDS)

clean:
	@rm -f $(MODULES) $(DEPENDS) $(PRODUCTS)
