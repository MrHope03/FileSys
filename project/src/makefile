files := $(wildcard *.c)

all: run del

run: $(files)
	gcc -o run $^
	./run

del:
	rm run

clean:
	rm ../files/*
