all: main
main: main.cpp
	g++ main.cpp -o main -Isrc/Include -Lsrc/lib -lSDL3

clean:
	rm -f main