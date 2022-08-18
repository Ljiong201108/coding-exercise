.PHONY: clean

test: tester.o library.o
	gcc -std=c11 -o2 tester.o library.o -o test

tester.o: tester.c library.h
	gcc -std=c11 -o2 -c tester.c -o tester.o

library.o: library.c library.h
	gcc -std=c11 -o2 -c library.c -o library.o

clean:
	rm -rf tester library.o tester.o