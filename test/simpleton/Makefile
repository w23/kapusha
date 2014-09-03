KAPUSHA_SIMPLE := 1
SOURCES += $(PRODUCT).c main_sdl.c
CFLAGS += -I./3p/SDL-linux64/include -D_REENTRANT
LDFLAGS += -L./3p/SDL-linux64/lib -lSDL2 -lpthread
all: $(PRODUCT)
include ../../kapusha.mk
