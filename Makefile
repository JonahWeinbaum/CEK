.PHONY: all clean
all: repl

repl: repl.c
	gcc -Wall -Wextra -pedantic -Os $^ -o $@ 

clean:
	rm -rf *.o repl
