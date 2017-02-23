# Change the following values to suit your system.

CFLAGS=-I/usr/include -I/usr/X11R6/include -L/usr/lib -L/usr/X11R6/lib -O2
LIB=-lX11 -lGL -lGLU -lglut -lm -lXmu -lXi
CCC=gcc

OBJECTS = 

run: main.o $(OBJECTS)
	$(CCC) $(CFLAGS) main.c -o run $(OBJECTS) $(LIB)

main.o: main.c
	$(CCC) $(CFLAGS) -c main.c

clean:
	rm -f run *.o
	rm -f *~
