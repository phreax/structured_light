INCPATH = -I/usr/include/opencv 
LIBPATH = -L/usr/lib
CFLAGS  = -O2 -g
LDFLAGS = -Wl,--rpath -Wl,/usr/lib -Wno-write-strings -lcv -lcvaux -lcxcore -lm -lhighgui -lm -lstdc++  -lGL -lglut -lm -lGLU
RM 		= /bin/rm -f
CC		= colorgcc 
SOURCES = three_step_phase_shift.cpp ThreeStepTest.cpp
OBJECTS = $(SOURCES:.cpp=.o)
MAIN	= tsptest

all: $(MAIN) 


$(MAIN): $(OBJECTS)
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

camTest: camTest.cpp 
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

glutExample: glutExample.cpp 
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

point_cloud: point_cloud.cpp 
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

.cpp.o:
	$(CC) $(INCPATH) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o

