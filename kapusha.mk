KAPUSHA_ROOT := $(dir $(lastword $(MAKEFILE_LIST)))
include $(KAPUSHA_ROOT)common.mk
OBJDIR ?= .obj

KAPUSHA_SRC = $(KAPUSHA_ROOT)src

# core
SOURCES += \
	$(KAPUSHA_SRC)/core/core.c

# math
SOURCES += \
	$(KAPUSHA_SRC)/math/math.c

ifeq ($(WITH_LINUX),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/linux.c
endif

ifeq ($(OS_RASPBERRY),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/rpi/VideoCore.c \
	$(KAPUSHA_SRC)/sys/rpi/EGL.c \
	$(KAPUSHA_SRC)/sys/rpi/RPi.c \
	$(KAPUSHA_SRC)/sys/rpi/Evdev.c
endif

ifeq ($(WITH_X11),1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/x11_systems.c \
	$(KAPUSHA_SRC)/sys/x11_main.c
endif

ifeq ($(WITH_GL),1)
SOURCES += \
	$(KAPUSHA_SRC)/render/gl.c
endif

#ifeq ($(WITH_SIMPLE_X11),1)
#SOURCES += \
#	$(KAPUSHA_SRC)/sys/x11_systems.c \
#	$(KAPUSHA_SRC)/sys/x11_main.c
#endif

ifeq ($(WITH_WIN32), 1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/win_systems.c
endif

ifeq ($(WITH_POSIX), 1)
SOURCES += \
	$(KAPUSHA_SRC)/sys/posix.c
endif

# Generate rules for .d and .o files
$(foreach src,$(SOURCES), $(eval $(call MAKE_RULES,$(src))))

-include $(DEPFILES)

$(PRODUCT): $(MODULES) $(METADEPS)
	$(CC) $(MODULES) $(LDFLAGS) -o $@

clean:
	@rm -f $(MODULES) $(DEPFILES) $(PRODUCT)
