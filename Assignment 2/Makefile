DEPS=server.h client.h client_test.h shared_memory_controller.h instruction.h thread_safe_map.h
OBJ=main.o server.o client.o client_test.o shared_memory_controller.o instruction.o thread_safe_map.o
CFLAG=-std=c++20 -O2 -Wall -Werror

.PHONY: clean

exec: ${OBJ}
	g++ ${CFLAG} -o $@ $^ 

%.o: %.cpp ${DEPS}
	g++ ${CFLAG} -c -o $@ $< 

clean:
	rm -rf ${OBJ} exec