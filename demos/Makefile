ROOT=..
KAPUSHA_ROOT=$(ROOT)
include $(ROOT)/common.mk

SAMPLES = \
	terrain
#	triangle \
	fullscreen_shader \
	flyby_dust \
	terrain \
	textosterone \
	pathtracer

all_samples: $(SAMPLES)

$(ROOT)/libkapusha.a:
	make -C $(ROOT)

$(ROOT)/libkapusha_main.a:
	make -C $(ROOT)

%: %.o $(ROOT)/libkapusha.a $(ROOT)/libkapusha_main.a
	make -C $(ROOT)
	$(LD) $< -lkapusha_main -lkapusha $(LDFLAGS) -L$(ROOT) -o $@

clean:
	@rm -f $(SAMPLES) *.o
