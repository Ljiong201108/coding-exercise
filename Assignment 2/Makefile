.PHONY: clean

thread_safe_map: main.o thread_safe_map.o
	g++ -std=c++20 -o2 main.o thread_safe_map.o -o thread_safe_map

main.o: main.cpp thread_safe_map.hpp
	g++ -std=c++20 -o2 -c main.cpp -o main.o

thread_safe_map.o: thread_safe_map.cpp thread_safe_map.hpp
	g++ -std=c++20 -o2 -c thread_safe_map.cpp -o thread_safe_map.o

clean:
	rm -rf main.o thread_safe_map.o thread_safe_map