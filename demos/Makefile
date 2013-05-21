ROOT=..
include $(ROOT)/common.mk

SAMPLES = triangle \
	fullscreen_shader \
	flyby_dust \
	terrain \
	pathtracer

RUNNER = main_sdl

all_samples: $(SAMPLES)

%: %.o $(RUNNER).o $(ROOT)/libkapusha.a
	$(LD) $< -lkapusha $(LDFLAGS) $(RUNNER).o -L$(ROOT) -o $@

clean:
	@rm -f $(SAMPLES) *.o
