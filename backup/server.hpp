#include <functional>
#include <thread>

#include "thread_safe_map.hpp"
#include "shared_memory_controller.hpp"

#define NUM_WORKER 10

class server{
public:
  server(size_t);

  void start();

private:
  thread_safe_map M;

  shared_memory_controller ctler;
};

server::server(size_t map_size) : M(map_size), ctler(true) {}

void server::start(){
  auto worker_process=[]{
    while(true){
      instruction inst=ctler.pop_instruction();

      if(inst.get_operator()==KILL) break;
      
      switch(inst.get_operator()){
        case PUT: 
          M.put(inst.get_operand().first, inst.get_operand().second);
          ctler.write_ret(inst.get_qid(), true, 0);
          break;

        case GET:
          optional<int> ret=M.get(inst.get_operand().first);
          if(ret.has_value()) ctler.write_ret(inst.get_qid(), true, ret.value());
          else ctler.write_ret(inst.get_qid(), false, 0);

        case REMOVE:
          optional<int> ret=M.remove(inst.get_operand().first);
          if(ret.has_value()) ctler.write_ret(inst.get_qid(), true, ret.value());
          else ctler.write_ret(inst.get_qid(), false, 0);
      }
    }
  };

  vector<thread> threads(NUM_WORKER, thread(worker_process));

  char c;
  cin>>c;
  for(int i=0;i<NUM_WORKER;i++) ctler.push_instruction(instruction(KILL, 0, 0));

  for(auto thread : threads) thread.join();
}