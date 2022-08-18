#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdexcept>

#include "instruction.hpp"

#define SHM_SIZE 4096
#define QUEUE_SIZE 1024
#define PATHNAME "."
#define KEY_SHM 0xACAC
#define KEY_SEM 0xCFCF

union semun{
  unsigned short *init_val;
}su;

class shared_memory_controller{
public:
  shared_memory_controller(bool);
  ~shared_memory_controller();

  void push_instruction();

  instruction pop_instruction();

private:
  bool is_creator;

  int id_shm, id_sem;

  char *addr;
};

shared_memory_controller::shared_memory_controller(bool is_creator) : is_creator(is_creator){
  id_shm=shmget(KEY_SHM, SHM_SIZE, IPC_CREAT|0666);
  if(id_shm<0) throw runtime_error("cannot create or get id for shared memory");
  addr=(char*)shmat(id_shm, NULL, 0);

  //ctl=0, empty=1, full=2
  id_sem=semget(KEY_SEM, 3, IPC_CREAT|0666);
  if(id_sem<0) throw runtime_error("cannot create or get id for ctl semaphore");

  unsigned short tmp[3]={1, QUEUE_SIZE, 0};
  su.init_val=(unsigned short*)&tmp;
  if(semctl(id_sem, 0, SETALL, su)<0) throw runtime_error("cannot set initial value to semaphores");
}

shared_memory_controller::~shared_memory_controller(){
  if(id_shm>=0){
    shmdt(addr);
    if(is_creator) shmctl(id_shm, IPC_RMID, NULL);
  }

  if(is_creator && id_sem>=0) semctl(id_sem, 0, IPC_RMID);
}