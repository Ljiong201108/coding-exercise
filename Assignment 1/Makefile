.PHONY: clean

test: tester.o library.o
	gcc tester.o library.o -o test

tester.o: tester.c library.h
	gcc -c tester.c -o tester.o

library.o: library.c library.h
	gcc -c library.c -o library.o

clean:
	rm tester library.o tester.o