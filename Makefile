INCPATH = -I/usr/include/opencv 
LIBPATH = -L/usr/lib
OPTIONS = -lcv -lcvaux -lcxcore -lm -lhighgui -lm -lstdc++ -Wl,--rpath -Wl,/usr/lib -Wno-write-strings
RM 		= /bin/rm -f
CC		= g++


all: camTest 

camTest: camTest.cpp 
	g++ $(INCPATH) $(LIBPATH) $(OPTIONS) $^ -o $@

clean:
	$(RM) *~ *.o
