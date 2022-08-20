DEPS=library.h
OBJ=test.o library.o
CFLAG=-std=c17 -O2 -Wall -Werror

.PHONY: clean

test: ${OBJ}
	gcc ${CFLAG} -o $@ $^

%.o: %.c ${DEPS}
	gcc ${CFLAG} -c -o $@ $< 

clean:
	rm -rf test library.o test.o