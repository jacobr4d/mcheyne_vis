all:   clean read

read: 
	clang -g -Wall -o read program.c parser.c

clean:
	rm -f read