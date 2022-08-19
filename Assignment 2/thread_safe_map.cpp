#include "thread_safe_map.h"

thread_safe_map::thread_safe_map(size_t size) : buckets(size){}

void thread_safe_map::put(int key, int value){
  unique_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, int> value){return value.first==key;});

  if(pos!=bucket.end()) pos->second=value;
  else bucket.push_back(make_pair(key, value));
}

optional<int> thread_safe_map::get(int key) const {
  shared_lock<shared_mutex> lck(mutex);
  const auto &bucket=buckets.at(key%buckets.size());
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, int> value){return value.first==key;});

  if(pos!=bucket.end()) return make_optional(pos->second);
  else return nullopt;
}

optional<int> thread_safe_map::remove(int key){
  unique_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  auto pos=find_if(bucket.begin(), bucket.end(), [key](pair<int, int> value){return value.first==key;});

  if(pos!=bucket.end()) {bucket.erase(pos); return make_optional(pos->second);}
  else return nullopt;
}