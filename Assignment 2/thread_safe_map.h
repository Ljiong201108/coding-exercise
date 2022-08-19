#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <cstddef>
#include <algorithm>
using namespace std;

class thread_safe_map{
public:
  thread_safe_map(size_t);

  void put(int, int);

  optional<int> get(int) const;

  optional<int> remove(int);

private:
  vector<list<pair<int, int>>> buckets;

  mutable shared_mutex mutex;
};