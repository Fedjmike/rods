ifeq ($(OS),Windows_NT)
	out_extension = .exe
endif

out_release = bin/release/rods$(out_extension)
out_debug = bin/debug/rods$(out_extension)

all: $(out_release)
debug: $(out_debug)

headers = $(wildcard src/*.hpp) $(wildcard src/*.hxx) $(wildcard src/*.h)
modules = $(wildcard src/*.cpp) $(wildcard src/*.cxx) $(wildcard src/*.c)
objects_release = $(patsubst src/%, obj/release/%.o, $(modules))
objects_debug = $(patsubst src/%, obj/debug/%.o, $(modules))

cflags = -Wall -Wextra -std=c++0x
cflags_release = -O3
ldflags = -lmingw32 -lSDLmain -lSDL

$(out_release): $(objects_release)
	g++ $(objects_release) $(ldflags) -o $(out_release)
	
$(out_debug): $(objects_debug)
	g++ $(objects_debug) $(ldflags) -o $(out_debug)

obj/release/%.o: src/% $(headers)
	g++ $(cflags) -c $< -o $@
	
obj/debug/%.o: src/% $(headers)
	g++ $(cflags) -c $< -o $@

clean:
	rm -f $(out_release)
	rm -f $(out_debug)
	rm -f obj/debug/*.o
	rm -f obj/release/*.o