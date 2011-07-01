INCPATH = -I/usr/include/opencv 
LIBPATH = -L/usr/lib
OPTIONS = -lcv -lcvaux -lcxcore -lm -lhighgui -lm -lstdc++ -Wl,--rpath -Wl,/usr/lib -Wno-write-strings -lGL -lglut -lm -lGLU
RM 		= /bin/rm -f
CC		= g++


all: camTest 

camTest: camTest.cpp 
	g++ $(INCPATH) $(LIBPATH) $(OPTIONS) $^ -o $@

glutExample: glutExample.cpp 
	g++ $(INCPATH) $(LIBPATH) $(OPTIONS) $^ -o $@

point_cloud: point_cloud.cpp 
	g++ $(INCPATH) $(LIBPATH) $(OPTIONS) $^ -o $@

clean:
	$(RM) *~ *.o

