.PHONY: clean

test: tester.o library.o
	gcc -std=c11 -o2 -Wall -Werror tester.o library.o -o test

tester.o: tester.c library.h
	gcc -std=c11 -o2 -Wall -Werror -c tester.c -o tester.o

library.o: library.c library.h
	gcc -std=c11 -o2 -Wall -Werror -c library.c -o library.o

clean:
	rm -rf test library.o tester.o