.PHONY: all clean

PRODUCTS += libkapusha.a

all: libkapusha.a

include kapusha.mk

libkapusha.a: $(MODULES) $(DEPENDS) $(METADEPS)
	@rm -f libkapusha.a
	$(AR) rcs libkapusha.a $(MODULES)
