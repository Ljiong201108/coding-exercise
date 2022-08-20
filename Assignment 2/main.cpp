#include <string>

#include "server.h"
#include "client.h"
#include "client_test.h"

void print_help(){
  cout<<"Use the format: [ ./exec [-C | -c] ] to create a client"<<endl;
  cout<<"Use the format: [ ./exec [-S | -s] size ] to create a server"<<endl;
  cout<<"Please start the server at first and stop the server at last!"<<endl;
}

int main(int argc, char** argv){
  if(argc<2){
    cout<<"invaild parameter format!"<<endl;
    print_help();
    return -1;
  }

  string type(argv[1]);
  if(type=="-C" || type=="-c"){
    if(argc>=3 && (string(argv[2])=="-t" || string(argv[2])=="-T"))
      try{
        client_test().start();
      }catch(const runtime_error &e){
        cout<<"An error occurs in the test client: "<<e.what()<<endl;
      }
    else
      try{
        client().start();
      }catch(const runtime_error &e){
        cout<<"An error occurs in the client: "<<e.what()<<endl;
      }
  }else{
    if(argc<3){
      cout<<"A size is needed!"<<endl;
      print_help();
      return -1;
    }

    int size;
    try{
      size=stoi(argv[2]);
    }catch(const exception &e){
      cout<<"error: size cannot convert to integal: "<<e.what()<<endl;
      return -1;
    }

    try{
      server(size).start();
    }catch(const runtime_error &e){
      cout<<"An error occurs in the server: "<<e.what()<<endl;
    }
  }
  
  return 0;
}