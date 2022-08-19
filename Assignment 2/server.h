#pragma once

#include <iostream>
#include <functional>

#include "thread_safe_map.h"
#include "shared_memory_controller.h"

#define NUM_WORKER 10

/**
 * proxy for server
 */
class server{
public:
  server(size_t);

  void start();

private:
  size_t size;
};