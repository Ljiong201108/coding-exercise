#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#define ONE_TIME_ALLOC_SIZE 1024

typedef struct block_ctrl_t block_ctrl_t;

struct block_ctrl_t{
  size_t size;
  block_ctrl_t *pre;
  block_ctrl_t *nxt;
  int is_free;
  int padding;
};

#define BLOCK_CTRL_SIZE sizeof(block_ctrl_t)

void *malloc(size_t);

void free(void*);

/*--------------for testing---------------*/



#endif