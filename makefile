all: rods

objects = main.o drawing.o
cflags = -Wall -Wextra -std=c++0x
ldflags = -lSDL -lSDLmain -lSDL_ttf

rods: $(objects)
	g++ $(objects) $(ldflags) -o rods

.cpp.o:
	g++ $(cflags) -c $< -o $@

clean:
	rm *.o
	rm rods