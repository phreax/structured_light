INCPATH = -I/usr/include/opencv 
LIBPATH = -L/usr/lib -L/usr/local/lib
CFLAGS  = -O2 -g
LDFLAGS = -Wl,--rpath -Wl,/usr/lib -Wno-write-strings  -lstdc++  -lGL -lglut -lm -lGLU
RM 		= /bin/rm -f
CC		= colorgcc 
SOURCES = three_step_phase_shift.cpp three_step_phase_shift.cpp
OBJECTS = $(SOURCES:.cpp=.o)
MAIN	= tpstest

all: $(MAIN) 


$(MAIN): $(OBJECTS)
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

cam_test: cam_test.cpp 
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

point_cloud: point_cloud.cpp 
	$(CC) $(INCPATH) $(LIBPATH) $(LDFLAGS) $(CFLAGS) $^ -o $@

.cpp.o:
	$(CC) $(INCPATH) $(CFLAGS) -c $< 

clean:
	$(RM) *~ *.o

