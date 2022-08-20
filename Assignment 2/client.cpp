#include "client.h"

void client::print_help(){
  cout<<"Please type command like this:"<<endl;
  cout<<"command [op1] [op2]"<<endl;
  cout<<"command list:"<<endl;
  cout<<"PUT(P, p) op1 op2"<<endl;
  cout<<"GET(G ,g) op1"<<endl;
  cout<<"REMOVE(R, r) op1"<<endl;
  cout<<"EXIT(E, e)"<<endl;
}

void client::start(){
  shared_memory_controller ctler(false);
  print_help();

  while(true){
    cout<<"> ";

    string command;
    int op1, op2;
    cin>>command;
    if(command=="P" || command=="p" || command=="PUT"){
      if(!(cin>>op1) || !(cin>>op2)){
        cout<<"! Operand type invaild"<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
      }

      unsigned short qid=ctler.push_instruction(instruction(PUT, op1, op2));
      ctler.read_ret(qid);
      cout<<"* Successfully put key-value pair ("<<op1<<", "<<op2<<") into map"<<endl;
    }else if(command=="G" || command=="g" || command=="GET"){
      if(!(cin>>op1)){
        cout<<"! Operand type false"<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
      }

      unsigned short qid=ctler.push_instruction(instruction(GET, op1, 0));
      pair<int, int> ret=ctler.read_ret(qid);

      if(ret.first) cout<<"* Value is "<<ret.second<<endl;
      else cout<<"! Key "<<op1<<" do not exist"<<endl;
    }else if(command=="R" || command=="r" || command=="REMOVE"){
      if(!(cin>>op1)){
        cout<<"! Operand type false"<<endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
      }

      unsigned short qid=ctler.push_instruction(instruction(REMOVE, op1, 0));
      pair<int, int> ret=ctler.read_ret(qid);

      if(ret.first) cout<<"* Successfully removed key "<<op1<<", its value is "<<ret.second<<endl;
      else cout<<"! Key "<<op1<<" do not exist"<<endl;
    }else if(command=="EXIT" || command=="E" || command=="e") 
      break;
    else 
      cout<<"! Command not found"<<endl;
  }
}