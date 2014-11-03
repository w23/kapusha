.SUFFIXES:

METADEPS = Makefile $(KAPUSHA_ROOT)kapusha.mk $(KAPUSHA_ROOT)common.mk

CFLAGS += -std=c99 -Wall -Werror -I$(KAPUSHA_ROOT)include
LDFLAGS += -lm -pthread

ifeq ($(DEBUG),1)
	CFLAGS += -O0 -g -DDEBUG=1
	WITH_STRIP := 0
else
	CFLAGS += -O2 -fomit-frame-pointer
	DO_STRIP ?= 1
endif

CXXFLAGS = $(CFLAGS) -std=c++11 -fno-exceptions -fno-rtti

# Detect platform
ifeq ($(OS),Windows_NT)
	PLATFORM := Windows
else # Not on windows
	# assume linux desktop
	# TODO other *nix, raspberry
	PLATFORM := LinuxDesktop
endif

# assign per-platform settings
ifeq ($(PLATFORM),LinuxDesktop)
	WITH_POSIX := 1
	WITH_X11 := 1
	WITH_GL := 1
	WITH_LINUX := 1
	CFLAGS += -DKP_GCC_ATOMICS
else ifeq ($(PLATFORM),LinuxRaspberry)
# On Raspberry Pi there are some specifics
	CFLAGS += -DKP_GCC_ATOMICS -DKP_RASPBERRY
	CFLAGS += -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
	CFLAGS += -I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -lGLESv2 -lEGL -lbcm_host -L/opt/vc/lib
# Only EGL is supported
	WITH_LINUX := 1
	WITH_POSIX := 1
	WITH_X11 := 0
	WITH_EGL := 1
	SYSTEM := sys_rpi.c
else ifeq ($(PLATFORM),Windows)
	CFLAGS += -DUNICODE -D_UNICODE
	LDFLAGS += -luser32 -lopengl32 -lgdi32 -lwinmm
	LDFLAGS += -static-libgcc -static-libstdc++
	WITH_WIN32 := 1
	WITH_LINUX := 0
	WITH_POSIX := 0
	WITH_X11 := 0
	WITH_EGL := 0
	SYSTEM := sys_win32.c
endif

ifeq ($(WITH_X11),1)
	LDFLAGS += -lX11 -lrt -lXrandr
endif

ifeq ($(WITH_GL),1)
	LDFLAGS += -lGL
endif

ifeq ($(WITH_POSIX),1)
	CFLAGS += -DKP_OS_POSIX
	CFLAGS += -pthread
	LDFLAGS += -pthread
endif

ifeq ($(DO_STRIP), 1)
	LDFLAGS += -s
endif

# TODO proceed with harftype
WITH_HARFTYPE := 0
ifeq ($(WITH_HARFTYPE),1)
	CFLAGS += `pkg-config --cflags freetype2 harfbuzz`
	LDFLAGS += `pkg-config --libs freetype2 harfbuzz`
endif

# oh god this is awful and cannot be explained!
define MAKE_RULES
  $1.MODULE := $(subst $(KAPUSHA_ROOT),kapusha/,$(addprefix $(OBJDIR)/, $(1:.c=.o)))
  $1.DEPFILE := $(subst $(KAPUSHA_ROOT),kapusha/,$(addprefix $(OBJDIR)/, $(1:.c=.d)))
  MODULES += $$($1.MODULE)
  DEPFILES += $$($1.DEPFILE)

$$($1.MODULE): $1 $$($1.DEPFILE) $(METADEPS)
	$(CC) $(CFLAGS) -c -o $$($1.MODULE) $1

$$($1.DEPFILE): $1 $(METADEPS)
	@mkdir -p $$(dir $$($1.DEPFILE))
	$(CC) $(CFLAGS) -MM $1 -MT $$($1.MODULE) -MT $$($1.DEPFILE) -MF $$($1.DEPFILE)
endef
