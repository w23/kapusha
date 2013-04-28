ROOT=..
include $(ROOT)/common.mk

SAMPLES = triangle \
	  fullscreen_shader \
	  flyby_dust

RUNNER = main_sdl

all_samples: $(SAMPLES)

%: %.o $(RUNNER).o
	$(LD) $(LDFLAGS) $(RUNNER).o $< -L$(ROOT) -lkapusha -o $@
