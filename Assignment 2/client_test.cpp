#include "client_test.h"

void client_test::start(){
  shared_memory_controller ctler(false);

  default_random_engine e;
  e.seed(time(0));
  uniform_int_distribution<int> gen_operator(1, 3), gen_operand(0, 1000);

  while(true){
    string command;
    int op=gen_operator(e), op1=gen_operand(e), op2=gen_operand(e);
    
    switch(op){
      case PUT: command="PUT"; break;
      case GET: command="GET"; break;
      case REMOVE: command="REMOVE"; break;
    }

    cout<<"[ "<<command<<" ";
    if(command=="P" || command=="p" || command=="PUT"){
      cout<<op1<<" "<<op2<<" ]"<<endl;

      unsigned short qid=ctler.push_instruction(instruction(PUT, op1, op2));

      ctler.read_ret(qid);
      cout<<"* Successfully put key-value pair ("<<op1<<", "<<op2<<") into map"<<endl;
    }else if(command=="G" || command=="g" || command=="GET"){
      cout<<op1<<" ]"<<endl;

      unsigned short qid=ctler.push_instruction(instruction(GET, op1, 0));
      pair<int, int> ret=ctler.read_ret(qid);

      if(ret.first) cout<<"* Value is "<<ret.second<<endl;
      else cout<<"! Key "<<op1<<" do not exist"<<endl;
    }else if(command=="R" || command=="r" || command=="REMOVE"){
      cout<<op1<<" ]"<<endl;

      unsigned short qid=ctler.push_instruction(instruction(REMOVE, op1, 0));
      pair<int, int> ret=ctler.read_ret(qid);

      if(ret.first) cout<<"* Successfully removed key "<<op1<<", its value is "<<ret.second<<endl;
      else cout<<"! Key "<<op1<<" do not exist"<<endl;
    }

    this_thread::sleep_for(50ms);
  }
}