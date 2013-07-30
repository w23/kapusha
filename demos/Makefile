ROOT=..
KAPUSHA_ROOT=$(ROOT)
include $(ROOT)/common.mk

SAMPLES = \
	triangle \
	fullscreen_shader \
	flyby_dust \
	terrain \
	textosterone \
	pathtracer

ifeq ($(KP_SDL),1)
	RUNNER = main_sdl
endif

ifeq ($(KP_X11),1)
	RUNNER = main_x11
endif

ifeq ($(KAPUSHA_WINDOWS),1)
	RUNNER = main_win32
endif

all_samples: $(SAMPLES)

%: %.o $(RUNNER).o $(ROOT)/libkapusha.a
	make -C $(ROOT)
	$(LD) $< $(RUNNER).o -lkapusha $(LDFLAGS) -L$(ROOT) -o $@

clean:
	@rm -f $(SAMPLES) *.o
