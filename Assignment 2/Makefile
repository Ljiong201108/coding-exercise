.PHONY: clean

thread_safe_map: main.o server.o client.o shared_memory_controller.o instruction.o thread_safe_map.o
	g++ -std=c++20 -o2 -Wall -Werror main.o server.o client.o shared_memory_controller.o instruction.o thread_safe_map.o -o exec

main.o: main.cpp server.h shared_memory_controller.h instruction.h
	g++ -std=c++20 -o2 -Wall -Werror -c main.cpp -o main.o

server.o: server.cpp server.h shared_memory_controller.h instruction.h thread_safe_map.o
	g++ -std=c++20 -o2 -Wall -Werror -c server.cpp -o server.o

client.o: client.cpp client.h shared_memory_controller.h instruction.h
	g++ -std=c++20 -o2 -Wall -Werror -c client.cpp -o client.o

shared_memory_controller.o: shared_memory_controller.cpp shared_memory_controller.h instruction.h
	g++ -std=c++20 -o2 -Wall -Werror -c shared_memory_controller.cpp -o shared_memory_controller.o

instruction.o: instruction.cpp instruction.h
	g++ -std=c++20 -o2 -Wall -Werror -c instruction.cpp -o instruction.o

thread_safe_map.o: thread_safe_map.cpp thread_safe_map.h
	g++ -std=c++20 -o2 -Wall -Werror -c thread_safe_map.cpp -o thread_safe_map.o

clean:
	rm -rf main.o instruction.o shared_memory_controller.o server.o client.o thread_safe_map.o exec