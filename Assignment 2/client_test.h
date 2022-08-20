#pragma once

#pragma once

#include <iostream>
#include <functional>
#include <limits>
#include <cctype>
#include <random>
#include <ctime>
#include <chrono>
#include <thread>

#include "shared_memory_controller.h"

/**
 * proxy for test client
 * it generates huge amount of queries per time unit to test the system stability
 */
class client_test{
public:
  void start();
};