#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <cstddef>
#include <algorithm>
using namespace std;

template<typename T=int>
class thread_safe_map{
private:
  vector<list<pair<int, T>>> buckets;

  mutable shared_mutex mutex;

public:
  thread_safe_map(size_t);

  void put(int, T);

  optional<T> get(int) const;

  optional<T> remove(int);
};

template<typename T>
thread_safe_map<T>::thread_safe_map(size_t size):buckets(size) {}

template<typename T>
void thread_safe_map<T>::put(int key, T value){
  unique_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, T> value){return value.first==key;});

  if(pos!=bucket.end()) pos->second=value;
  else bucket.push_back(make_pair(key, value));
}

template<typename T>
optional<T> thread_safe_map<T>::get(int key) const {
  shared_lock<shared_mutex> lck(mutex);
  const auto &bucket=buckets.at(key%buckets.size());
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, T> value){return value.first==key;});

  if(pos!=bucket.end()) return make_optional(pos->second);
  else return nullopt;
}

template<typename T>
optional<T> thread_safe_map<T>::remove(int key){
  unique_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, T> value){return value.first==key;});

  if(pos!=bucket.end()) {bucket.erase(pos); return make_optional(pos->second);}
  else return nullopt;
}