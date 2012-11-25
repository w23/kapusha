include ../common.mk

flyby_static_geometry: flyby_static_geometry.cpp ../libKapusha.a
	$(CC) $(CXXFLAGS) $(LDFLAGS) flyby_static_geometry.cpp -L.. -lKapusha -lstdc++ -lGL -lglut -o flyby_static_geometry
