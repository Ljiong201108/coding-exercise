#include <iostream>
#include <thread>
#include <chrono>

#include "thread_safe_map.hpp"
#include "shared_memory_controller.hpp"

int main(){
  shared_memory_controller shm(1);
}