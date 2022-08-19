#pragma once

#pragma once

#include <iostream>
#include <functional>
#include <limits>
#include <cctype>

#include "shared_memory_controller.h"

/**
 * proxy for client
 */
class client{
public:
  void print_help();

  void start();
};