build:
	g++ -Wall -O0 -std=c++20 -pedantic -fsanitize=address -g main.cpp -o bin/run

build_mem:
	g++ -Wall -O0 -std=c++20 -pedantic -g main.cpp -o bin/run

run: build
	exec ./bin/run

debug: build 
	gdb bin/run 
