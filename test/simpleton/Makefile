KAPUSHA_SIMPLE := 1
SOURCES += $(PRODUCT).c
CFLAGS += -I$(KAPUSHA_ROOT)/3p/glfw/include
LDFLAGS += -L$(KAPUSHA_ROOT)/3p/glfw/build/src -lglfw3 -lX11 -lXrandr -lXi -lXxf86vm -lXcursor
all: $(PRODUCT)
include ../../kapusha.mk
