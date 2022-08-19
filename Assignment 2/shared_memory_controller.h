#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdexcept>
#include <iostream>

#include "instruction.h"

#define SHM_SIZE 4096
#define SHM_INT (SHM_SIZE/sizeof(int))
#define QUEUE_HEAD_ADDR (SHM_INT-2)
#define QUEUE_TAIL_ADDR (SHM_INT-1)
#define QUEUE_SIZE 200
#define KEY_SHM 0x3333
#define KEY_SEM 0x4444
#define KEY_RET_EMPTY 0x5555
#define KEY_RET_FILLED 0x6666
#define CTL 0
#define EMPTY 1
#define FILLED 2

class shared_memory_controller{
public:
  shared_memory_controller(bool);
  ~shared_memory_controller();

  unsigned short push_instruction(instruction);

  instruction pop_instruction();

  void write_ret(int, int, int);

  pair<int, int> read_ret(int);

  void print();

private:
  bool is_server;

  int id_shm, id_sem, id_ret_empty, id_ret_filled;

  int *addr;

  void sem_post(int, unsigned short);

  void sem_wait(int, unsigned short);
};