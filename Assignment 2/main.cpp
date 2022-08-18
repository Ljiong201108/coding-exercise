#include <iostream>

#include "thread_safe_map.hpp"

int main(){
  thread_safe_map M(5);
  M.put(5, 3);
  M.put(4, 2);
  cout<<M.get(5).value()<<endl;
  cout<<M.get(3).has_value()<<endl;
  cout<<M.get(4).value()<<endl;
  cout<<M.remove(4).value()<<endl;
  cout<<M.get(4).has_value()<<endl;
}