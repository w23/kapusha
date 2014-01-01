KAPUSHA_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))
include $(KAPUSHA_ROOT)common.mk
OBJDIR ?= obj

KAPUSHA_SRC = $(KAPUSHA_ROOT)kapusha

SOURCES += \
	$(KAPUSHA_SRC)/core/assert.cpp \
	$(KAPUSHA_SRC)/core/shared.cpp \
	$(KAPUSHA_SRC)/core/Object.cpp \
	$(KAPUSHA_SRC)/core/log.cpp \
	$(KAPUSHA_SRC)/core/Array.cpp \
	$(KAPUSHA_SRC)/core/list.cpp \
	$(KAPUSHA_SRC)/core/Surface.cpp \
	$(KAPUSHA_SRC)/core/String.cpp \
	$(KAPUSHA_SRC)/core/buffer.cpp

SOURCES += \
	$(KAPUSHA_SRC)/math/math.cpp

SOURCES += \
	$(KAPUSHA_SRC)/render/Program.cpp \
	$(KAPUSHA_SRC)/render/Material.cpp \
	$(KAPUSHA_SRC)/render/Batch.cpp \
	$(KAPUSHA_SRC)/render/Sampler.cpp \
	$(KAPUSHA_SRC)/render/State.cpp \
	$(KAPUSHA_SRC)/render/Buffer.cpp \
	$(KAPUSHA_SRC)/render/OpenGL.cpp \
	$(KAPUSHA_SRC)/render/Context.cpp \
	$(KAPUSHA_SRC)/render/Framebuffer.cpp

SOURCES += \
	$(KAPUSHA_SRC)/ooo/Reframe.cpp \
	$(KAPUSHA_SRC)/ooo/Camera.cpp

SOURCES += \
	$(KAPUSHA_SRC)/utils/SpectatorCameraController.cpp \
	$(KAPUSHA_SRC)/utils/geometry.cpp \
	$(KAPUSHA_SRC)/utils/Atlas.cpp \
	$(KAPUSHA_SRC)/utils/noise.cpp

ifeq ($(OS_RASPBERRY),1)
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
	$(KAPUSHA_SRC)/sys/x11/X11Pointers.cpp \
	$(KAPUSHA_SRC)/sys/x11/X11Keys.cpp \
	$(KAPUSHA_SRC)/sys/x11/main.cpp
endif

ifeq ($(OS_WINDOWS), 1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/win/main.cpp \
	$(KAPUSHA_SRC)/sys/win/WGLContext.cpp \
	$(KAPUSHA_SRC)/sys/win/WindowController.cpp \
	$(KAPUSHA_SRC)/sys/win/WindowsKeys.cpp \
	$(KAPUSHA_SRC)/sys/win/WindowsPointers.cpp
endif

ifeq ($(OS_POSIX), 1)
SOURCES += \
	$(KAPUSHA_SRC)/io/Socket_nix.cpp
endif

ifeq ($(WITH_HARFTYPE), 1)
SOURCES += \
	$(KAPUSHA_SRC)/fontain/harftype/Freetype.cpp \
	$(KAPUSHA_SRC)/fontain/harftype/Face.cpp
endif

# Generate rules for .d and .o files
$(foreach src,$(SOURCES), $(eval $(call MAKE_RULES,$(src))))

-include $(DEPFILES)

$(PRODUCT): $(MODULES) $(METADEPS)
	$(CXX) $(MODULES) $(LDFLAGS) -o $@

clean:
	@rm -f $(MODULES) $(DEPFILES) $(PRODUCT)
