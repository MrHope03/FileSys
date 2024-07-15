files := $(wildcard ./src/*.c)

compile: $(files)
	gcc -o run $^ -g

run: compile
	./run test

debug: compile
	gdb run

