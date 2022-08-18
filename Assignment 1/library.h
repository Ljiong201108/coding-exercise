#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>

#define ONE_TIME_ALLOC_SIZE 1024

typedef struct block_ctrl_t block_ctrl_t;

/**
 * the control module for a block, after this follows the data field controled by this module
 */
struct block_ctrl_t{
  size_t size;
  block_ctrl_t *pre;
  block_ctrl_t *nxt;
  int is_free;
  int padding; // 8 byte aligned
};

#define BLOCK_CTRL_SIZE sizeof(block_ctrl_t)

void *ts_malloc(size_t);

void ts_free(void*);

/*--------------for testing---------------*/

void test_print_all();

void test_print_head();

void test_print_tail();

void test_print(block_ctrl_t *block_ctrl);

block_ctrl_t *get_head();

block_ctrl_t *get_tail();

#endif