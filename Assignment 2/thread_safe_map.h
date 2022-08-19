#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <cstddef>
#include <algorithm>
using namespace std;

/**
 * The thread safe map. Use the read/write lock to realize the three basic concurrent operations: put, get and remove
 */
class thread_safe_map{
public:
  thread_safe_map(size_t);

  void put(int key, int value);

  optional<int> get(int key) const;

  optional<int> remove(int key);

private:
  vector<list<pair<int, int>>> buckets;

  mutable shared_mutex mutex;
};