include common.mk

SOURCES := \
	gl/Batch.cpp \
	gl/Buffer.cpp \
	gl/Camera.cpp \
	gl/Material.cpp \
	gl/Object.cpp \
	gl/OpenGL.cpp \
	gl/Program.cpp \
	gl/Texture.cpp \
	io/Stream.cpp \
	io/StreamFile_posix.cpp \
	math/math.cpp \
	sys/IViewport.cpp \
	sys/Log.cpp \
	sys/runGlut.cpp 

#MODULES=$(addprefix build/, $(patsubst %.cpp, %.o, $(SOURCES)))
MODULES=$(patsubst %.cpp, %.o, $(SOURCES))

.PHONY: all clean

libkapusha.a: $(MODULES)
	ar rcs libkapusha.a $(MODULES)

clean:
	@rm -rf $(MODULES) libkapusha.a
