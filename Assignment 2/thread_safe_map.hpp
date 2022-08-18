#pragma once

#include <vector>
#include <list>
#include <thread>
#include <shared_mutex>
#include <optional>
using namespace std;

template<typename T>
class thrad_safe_map{
private:
  vector<list<pair<int, T>>> buckets;

  mutable shared_mutex mutex;

public:
  void put(int, const T&);

  optional<T> get(int) const;

  optional<T> remove(int);
};