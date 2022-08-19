#include <string>

#include "server.h"
#include "client.h"

void print_help(){
  cout<<"Use the format: ./exec [-C] | [-S  size]"<<endl;
  cout<<"Start the server at first and stop the server at last!"<<endl;
  cout<<"Otherwise the shared memory will always be occupied!"<<endl;
}

int main(int argc, char** argv){
  bool is_server=false, is_client=false, exist_size=false;
  int size=0;
  for(int i=1;i<argc;i++){
    string arg(argv[i]);
    if(arg[0]!='-'){
      cout<<"invaild parameter!"<<endl;
      print_help();
      return -1;
    }

    arg=arg.substr(1);
    if(arg=="C") is_client=true;
    else if(arg=="S"){
      is_server=true;
      if(i!=argc-1){
        try{
          size=stoi(argv[i+1]);
        }catch(const exception &e){
          cout<<"error: size cannot convert to integal: "<<e.what()<<endl;
          return -1;
        }
        exist_size=true;
        i++;
      }
    }else{
      cout<<"invaild args!"<<endl;
      print_help();
      return -1;
    }
  }

  if(is_client && is_server){
    cout<<"Cannot be both server and client!"<<endl;
    print_help();
    return -1;
  }else if(is_server){
    if(!exist_size){
      cout<<"No size given!"<<endl;
      print_help();
      return -1;
    }

    try{
      server(size).start();
    }catch(const runtime_error &e){
      cout<<e.what()<<endl;
    }
  }else if(is_client){
    try{
      client().start();
    }catch(const runtime_error &e){
      cout<<e.what()<<endl;
    }
  }else{
    cout<<"Cannot be neither server nor client!"<<endl;
    print_help();
    return -1;
  }

  return 0;
}