ROOT=../../..
include $(ROOT)/common.mk

SOURCES = \
	GLXContext.cc \
	GLXViewportController.cc \
	x11.cc

MODULES=$(patsubst %.cc, %.o, $(SOURCES))

.PHONY: all clean

libkapusha_x11.a: $(MODULES)
	ar rcs libkapusha_x11.a $(MODULES)

clean:
	@rm -f $(MODULES) libkapusha_x11.a
