.SUFFIXES:
CFLAGS ?=
METADEPS = Makefile $(KAPUSHA_ROOT)/kapusha.mk $(KAPUSHA_ROOT)/common.mk
CXXFLAGS += $(CFLAGS) -std=c++11 -Wall -Werror -fno-exceptions -fno-rtti -I$(KAPUSHA_ROOT)
LDFLAGS += -lm

RELEASE ?= 0
OS ?= Linux
OS_RASPBERRY ?= 0
WITH_STRIP ?= 1

# If not release, then debug!
ifneq ($(RELEASE),1)
	DEBUG := 1
	CXXFLAGS += -O0 -g -DDEBUG=1
	WITH_STRIP := 0
else
	CXXFLAGS += -Os -fomit-frame-pointer
endif

# Detect windows and proceed WITH_ defaults
ifeq ($(OS),Windows_NT)
	OS_WINDOWS := 1
	OS_POSIX := 0
	WITH_X11 := 0
	WITH_EGL := 0
	WITH_SDL ?= 0
else # Not on windows
	OS_WINDOWS := 0
	OS_POSIX := 1
	WITH_X11 ?= 1
	WITH_EGL ?= 0
	WITH_SDL ?= 0
	WITH_HARFTYPE ?= 1
endif

# If SDL is specifically on
ifneq ($(WITH_SDL),1)
	WITH_X11 := 1
	WITH_EGL := 0
endif # With SDL

# On Raspberry Pi there are some specifics
ifeq ($(OS_RASPBERRY),1)
	CXXFLAGS += -DKP_RASPBERRY=1 -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads
	CXXFLAGS += -I/opt/vc/include/interface/vmcs_host/linux
	LDFLAGS += -lGLESv2 -lEGL -lbcm_host -L/opt/vc/lib
# Only EGL is supported
	WITH_SDL := 0
	WITH_X11 := 0
	WITH_EGL := 1
endif # On Raspberry

ifeq ($(WITH_X11),1)
	LDFLAGS += -lGL -lX11 -lrt
endif

ifeq ($(WITH_SDL),1)
	CXXFLAGS += `pkg-config --cflags sdl`
	LDFLAGS += `pkg-config --libs sdl` -lGL
endif

ifeq ($(OS_WINDOWS),1)
# TODO get rid of glew
	CFLAGS += -I$(KAPUSHA_ROOT)/3p/glew -DUNICODE -D_UNICODE -DGLEW_STATIC
	LDFLAGS += -luser32 -lopengl32 -lgdi32 -lwinmm
	LDFLAGS += -static-libgcc -static-libstdc++
endif

ifeq ($(WITH_STRIP), 1)
	LDFLAGS += -s
endif

# TODO proceed with harftype
WITH_HARFTYPE := 0
ifeq ($(WITH_HARFTYPE),1)
	CXXFLAGS += `pkg-config --cflags freetype2 harfbuzz`
	LDFLAGS += `pkg-config --libs freetype2 harfbuzz`
endif

# oh god this is awful and cannot be explained!
define MAKE_RULES
  $1.MODULE := $(addprefix $(OBJDIR)/, $(1:.cpp=.o))
  $1.DEPFILE := $(addprefix $(OBJDIR)/, $(1:.cpp=.d))
  MODULES += $$($1.MODULE)
  DEPFILES += $$($1.DEPFILE)

$$($1.MODULE): $1 $$($1.DEPFILE) $(METADEPS)
	$(CXX) $(CXXFLAGS) -c -o $$($1.MODULE) $1

$$($1.DEPFILE): $1 $(METADEPS)
	@mkdir -p $$(dir $$($1.DEPFILE))
	$(CXX) $(CXXFLAGS) -MM $1 -MT $$($1.MODULE) -MF $$($1.DEPFILE)
endef
