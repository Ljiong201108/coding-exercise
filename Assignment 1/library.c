#include "library.h"

block_ctrl_t *head, *tail;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

/**
 * @return return the pointer which points to the data field after the control block
 */
static void *get_data(block_ctrl_t *block_ctrl){
  return (void*)(block_ctrl+1);
}

/**
 * 
 * @return return the pointer which points to the control block before the data field
 */
static block_ctrl_t *get_block_ctrl(void *data){
  return (block_ctrl_t*)data-1;
}

/**
 * iterator through the linked list in order to find a worst fit
 * @return return the pointer to the control block whose data field fits. If no such control block exists, return NULL
 */
static block_ctrl_t *find_fit(size_t fit_size){
  block_ctrl_t *worst_fit=NULL;
  for(block_ctrl_t *iter=head;iter!=NULL;iter=iter->nxt){
    if(iter->is_free){
      if(worst_fit==NULL) worst_fit=iter;
      else if(iter->size>worst_fit->size) worst_fit=iter;
    } 
  }
  return (worst_fit!=NULL && worst_fit->size>=fit_size) ? worst_fit : NULL;
}

/**
 * split the data field of the given control block into the block with given size and the rest
 */ 
static void split_and_occupy_block(block_ctrl_t *block_ctrl, size_t size){
  //there is no meaning to cut if the rest area is even not able to hold a control block
  if(block_ctrl->size-size<=BLOCK_CTRL_SIZE){
    block_ctrl->is_free=0;
    return;
  }

  block_ctrl_t *rest_block_ctrl=(block_ctrl_t*)((char*)(block_ctrl+1)+size);
  rest_block_ctrl->pre=block_ctrl;
  rest_block_ctrl->nxt=block_ctrl->nxt;
  rest_block_ctrl->size=block_ctrl->size-size-BLOCK_CTRL_SIZE;
  rest_block_ctrl->is_free=1;
  block_ctrl->size=size;
  block_ctrl->is_free=0;
  block_ctrl->nxt=rest_block_ctrl;
  if(block_ctrl==tail) tail=rest_block_ctrl;
  else rest_block_ctrl->nxt->pre=rest_block_ctrl;
}

/**
 * if no block can fit, we should use sbrk() to get more area
 * @return return whether it is successful to get more place from the OS
 */ 
static int append_new_area(size_t needed_size){
  //if tail is a free block, we do not need to create a new free block, just add into it
  //here needed_size must be positive because we assume that no block can fit => the size of all block is lower than needed size
  if(tail!=NULL && tail->is_free) needed_size-=tail->size;
  //otherwise we need to create a new free block behind tail
  else needed_size+=BLOCK_CTRL_SIZE;

  //try to get more area than needed for further usage by ceiling the needed size to multiple of 1024
  size_t alloc_size=(needed_size/ONE_TIME_ALLOC_SIZE+1)*ONE_TIME_ALLOC_SIZE;

  block_ctrl_t *new_area;
  if((new_area=sbrk(alloc_size))==(void*)-1) return 0;
  
  if(tail!=NULL && tail->is_free) tail->size+=alloc_size;
  else{
    new_area->is_free=1;
    new_area->size=alloc_size-BLOCK_CTRL_SIZE;
    new_area->pre=tail;
    new_area->nxt=NULL;
    if(tail!=NULL) tail->nxt=new_area;
    tail=new_area;
    if(head==NULL) head=new_area;
  }

  return 1;
}

/**
 * make a block free and try to merge with neighbors
 * we can prove that if we keep merging a block with neighbors after it is freed, there will be no two continuous blocks that are both free
 * under the assumption that there is no race condition
 */ 
static void free_and_merge_block(block_ctrl_t *block_ctrl){
  block_ctrl->is_free=1;
  block_ctrl_t *left_block_ctrl=block_ctrl->pre, *right_block_ctrl=block_ctrl->nxt;

  if(left_block_ctrl!=NULL && left_block_ctrl->is_free){
    left_block_ctrl->nxt=right_block_ctrl;
    if(right_block_ctrl!=NULL) right_block_ctrl->pre=left_block_ctrl;
    left_block_ctrl->size+=block_ctrl->size+BLOCK_CTRL_SIZE;
    if(block_ctrl==tail) tail=left_block_ctrl;
    block_ctrl=left_block_ctrl;
  }

  if(right_block_ctrl!=NULL && right_block_ctrl->is_free){
    block_ctrl->nxt=right_block_ctrl->nxt;
    if(right_block_ctrl->nxt!=NULL) right_block_ctrl->nxt->pre=block_ctrl;
    block_ctrl->size+=right_block_ctrl->size+BLOCK_CTRL_SIZE;
    if(right_block_ctrl==tail) tail=block_ctrl;
  }
}

void *ts_malloc(size_t size){
  pthread_mutex_lock(&mutex);

  if(size<=0){
    pthread_mutex_unlock(&mutex);
    return NULL;
  }

  block_ctrl_t *block_ctrl;
  while(!(block_ctrl=find_fit(size))){
    if(!append_new_area(size)){
      pthread_mutex_unlock(&mutex);
      return NULL;
    }
  }

  split_and_occupy_block(block_ctrl, size);

  pthread_mutex_unlock(&mutex);
  return get_data(block_ctrl);
}

void ts_free(void* ptr){
  if(ptr==NULL) return;
  pthread_mutex_lock(&mutex);
  free_and_merge_block(get_block_ctrl(ptr));
  pthread_mutex_unlock(&mutex);
}

/*--------------for testing---------------*/

void test_print_head(){
  if(head==NULL) printf("Head is NULL\n");
  else printf("Block head: size=%lu, is_free: %s\n", head->size, head->is_free ? "true" : "false");
}

void test_print_tail(){
  if(tail==NULL) printf("Tail is NULL\n");
  else printf("Block tail: size=%lu, is_free: %s\n", tail->size, tail->is_free ? "true" : "false");
}

void test_print_all(){
  int cnt=0;
  for(block_ctrl_t *iter=head;iter!=NULL;iter=iter->nxt){
    printf("Block %d: size=%lu, is_free: %s\n", cnt++, iter->size, iter->is_free ? "true" : "false");
  }
  printf("\n");

  // for(block_ctrl_t *iter=tail;iter!=NULL;iter=iter->pre){
  //   printf("Block %d: size=%lu, is_free: %s\n", --cnt, iter->size, iter->is_free ? "true" : "false");
  // }
  // printf("\n");
}

void test_print(block_ctrl_t *block_ctrl){
  if(block_ctrl==NULL) printf("Block is NULL\n");
  else printf("Block: size=%lu, is_free: %s\n", block_ctrl->size, block_ctrl->is_free ? "true" : "false");
}

block_ctrl_t *get_head(){
  return head;
}

block_ctrl_t *get_tail(){
  return tail;
}