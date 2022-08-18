#include "thread_safe_map.hpp"

template<typename T>
void thread_safe_map<T>::put(int key, const T& value){
  unique_lock<shared_mutex> lck(mutex);
  buckets[key%buckets.size()].push_back(make_pair<int, T>(key, value));
}

template<typename T>
optional<T> thread_safe_map<T>::get(int key) const {
  shared_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  list<T>::iterator<T> pos;
  if((pos=find_if(bucket.begin(), bucket.end(), [](pair<int, T> value){return value.first==key}))!=bucket.end()) return make_optional<T>(pos->second);
  else return nullopt;
}

template<typename T>
optional<T> thread_safe_map<T>::remove(int key){
  unique_lock<shared_mutex> lck(mutex);
  auto &bucket=buckets[key%buckets.size()];
  list<T>::iterator pos;
  if((pos=find_if(bucket.begin(), bucket.end(), [](pair<int, T> value){return value.first==key}))!=bucket.end()){
    bucket.erase(pos);
    return make_optional<T>(pos->second);
  }else return nullopt;
}