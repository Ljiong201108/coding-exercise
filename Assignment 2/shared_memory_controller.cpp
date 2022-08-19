#include "shared_memory_controller.h"

shared_memory_controller::shared_memory_controller(bool is_server) : is_server(is_server){
  id_shm=shmget(KEY_SHM, SHM_SIZE, IPC_CREAT|0666);
  if(id_shm<0) throw runtime_error("cannot create or get id for shared memory");
  addr=(int*)shmat(id_shm, NULL, 0);
  addr[QUEUE_HEAD_ADDR]=0;
  addr[QUEUE_TAIL_ADDR]=0;

  id_sem=semget(KEY_SEM, 3, IPC_CREAT|0666);
  if(id_sem<0) throw runtime_error("cannot create or get id for ctl semaphore");
  
  id_ret_empty=semget(KEY_RET_EMPTY, QUEUE_SIZE, IPC_CREAT|0666);
  if(id_ret_empty<0) throw runtime_error("cannot create or get id for ret semaphore");

  id_ret_filled=semget(KEY_RET_FILLED, QUEUE_SIZE, IPC_CREAT|0666);
  if(id_ret_filled<0) throw runtime_error("cannot create or get id for ret semaphore");

  if(is_server){
    union semun{unsigned short *init_val;} su, su_ret_empty, su_ret_filled;
    unsigned short tmp[3]={1, QUEUE_SIZE, 0};
    su.init_val=(unsigned short*)&tmp;
    if(semctl(id_sem, 0, SETALL, su)<0) throw runtime_error("cannot set initial value to ctl semaphores");

    unsigned short tmp_ret_empty[QUEUE_SIZE];
    for(int i=0;i<QUEUE_SIZE;i++) tmp_ret_empty[i]=1;
    su_ret_empty.init_val=(unsigned short*)&tmp_ret_empty;
    if(semctl(id_ret_empty, 0, SETALL, su_ret_empty)<0) throw runtime_error("cannot set initial value to ret semaphores");

    unsigned short tmp_ret_filled[QUEUE_SIZE];
    for(int i=0;i<QUEUE_SIZE;i++) tmp_ret_filled[i]=0;
    su_ret_filled.init_val=(unsigned short*)&tmp_ret_filled;
    if(semctl(id_ret_filled, 0, SETALL, su_ret_filled)<0) throw runtime_error("cannot set initial value to ret semaphores");
  }
}

shared_memory_controller::~shared_memory_controller(){
  if(id_shm>=0){
    shmdt(addr);
    if(is_server) shmctl(id_shm, IPC_RMID, NULL);
  }

  if(is_server && id_sem>=0) semctl(id_sem, 0, IPC_RMID);
  if(is_server && id_ret_empty>=0) semctl(id_ret_empty, 0, IPC_RMID);
  if(is_server && id_ret_filled>=0) semctl(id_ret_filled, 0, IPC_RMID);
}

void shared_memory_controller::sem_post(int id, unsigned short num){
  struct sembuf op[1]={{num, 1, 0}};
  semop(id, op, 1);
}

void shared_memory_controller::sem_wait(int id, unsigned short num){
  struct sembuf op[1]={{num, -1, 0}};
  semop(id, op, 1);
}

unsigned short shared_memory_controller::push_instruction(instruction inst){
  sem_wait(id_sem, EMPTY);
  sem_wait(id_sem, CTL);

  int ret=addr[QUEUE_TAIL_ADDR]/INSTRUCTION_SIZE;
  int *pos_tail=&addr[addr[QUEUE_TAIL_ADDR]];
  pos_tail[0]=inst.get_operator();
  pos_tail[1]=inst.get_operand().first;
  pos_tail[2]=inst.get_operand().second;
  addr[QUEUE_TAIL_ADDR]=(addr[QUEUE_TAIL_ADDR]+INSTRUCTION_SIZE)%QUEUE_SIZE;

  sem_post(id_sem, CTL);
  sem_post(id_sem, FILLED);

  return ret;
}

instruction shared_memory_controller::pop_instruction(){
  sem_wait(id_sem, FILLED);
  sem_wait(id_sem, CTL);

  int *pos_head=&addr[addr[QUEUE_HEAD_ADDR]];
  instruction inst(pos_head[0], pos_head[1], pos_head[2], addr[QUEUE_HEAD_ADDR]/INSTRUCTION_SIZE);
  addr[QUEUE_HEAD_ADDR]=(addr[QUEUE_HEAD_ADDR]+INSTRUCTION_SIZE)%QUEUE_SIZE;

  sem_post(id_sem, CTL);
  sem_post(id_sem, EMPTY);
  return inst;
}

void shared_memory_controller::write_ret(int qid, int status, int ret){
  int base=QUEUE_SIZE*INSTRUCTION_SIZE+qid*RETURN_SIZE;

  sem_wait(id_ret_empty, qid);

  addr[base]=status;
  addr[base+1]=ret;

  sem_post(id_ret_filled, qid);
}

pair<int, int> shared_memory_controller::read_ret(int qid){
  int base=QUEUE_SIZE*INSTRUCTION_SIZE+qid*RETURN_SIZE;

  sem_wait(id_ret_filled, qid);

  pair<int, int> ret=make_pair(addr[base], addr[base+1]);

  sem_post(id_ret_empty, qid);

  return ret;
}

void shared_memory_controller::print(){
  for(size_t i=0;i<SHM_INT;i++){
    cout<<addr[i]<<" ";
    if(i%32==31) cout<<endl;
  }

  cout<<"ctl semaphore:"<<endl;
  for(int i=0;i<3;i++) cout<<semctl(id_sem, i, GETVAL, 0)<<" ";
  cout<<endl;

  cout<<"ret_empty semaphore:"<<endl;
  for(int i=0;i<QUEUE_SIZE;i++) cout<<semctl(id_ret_empty, i, GETVAL, 0)<<" ";
  cout<<endl;

  cout<<"ret_full semaphore:"<<endl;
  for(int i=0;i<QUEUE_SIZE;i++) cout<<semctl(id_ret_filled, i, GETVAL, 0)<<" ";
  cout<<endl;
}