files := $(wildcard ./src/*.c)

compile: $(files)
	gcc -o takenote $^ -g

run: compile
	./run test

debug: compile
	gdb run

