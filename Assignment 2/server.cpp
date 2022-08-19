#include "server.h"

server::server(size_t size) : size(size) {}

void server::start(){
  thread_safe_map M(size);
  shared_memory_controller ctler(true);

  vector<thread> threads;
  for(int i=0;i<NUM_WORKER;i++) threads.emplace_back([&ctler, &M]{
    while(true){
      instruction inst=ctler.pop_instruction();

      if(inst.get_operator()==KILL) break;
      else if(inst.get_operator()==PUT){
        M.put(inst.get_operand().first, inst.get_operand().second);
        ctler.write_ret(inst.get_qid(), true, 0);
      }else if(inst.get_operator()==GET){
        optional<int> ret=M.get(inst.get_operand().first);
        if(ret.has_value()) ctler.write_ret(inst.get_qid(), true, ret.value());
        else ctler.write_ret(inst.get_qid(), false, 0);
      }else if(inst.get_operator()==REMOVE){
        optional<int> ret=M.remove(inst.get_operand().first);
        if(ret.has_value()) ctler.write_ret(inst.get_qid(), true, ret.value());
        else ctler.write_ret(inst.get_qid(), false, 0);
      }
    }
  });

  cout<<"Server started, press ANY CHAR + ENTER to stop"<<endl;

  cin.get();
  for(int i=0;i<NUM_WORKER;i++) ctler.push_instruction(instruction(KILL, 0, 0));

  for(int i=0;i<NUM_WORKER;i++) threads[i].join();
}