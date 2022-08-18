#include <string.h>
#include <stdlib.h>

#include "library.h"

#define NUMBER 113

void simple_thread_test_case_1(){
  printf("Test Case 1:\n");
  printf("1. Allocate a field of 30 byte: \n");
  char* seg0=ts_malloc(30);
  for(int i=0;i<30;i++) seg0[i]=NUMBER;
  test_print_all();

  printf("2. Allocate two fields of 200 and 300 byte: \n");
  char* seg1=ts_malloc(200);
  for(int i=0;i<200;i++) seg1[i]=NUMBER;
  char* seg2=ts_malloc(300);
  for(int i=0;i<300;i++) seg2[i]=NUMBER;
  test_print_all();

  printf("3. Allocate a field of 40 byte: \n");
  char* seg3=ts_malloc(40);
  for(int i=0;i<40;i++) seg3[i]=NUMBER;
  test_print_all();

  printf("4. Free seg2: \n");
  ts_free(seg2);
  test_print_all();

  printf("5. Allocate a field of 50 byte: (It should be allocated in Block 2 because of worst fit)\n");
  char* seg5=ts_malloc(50);
  for(int i=0;i<50;i++) seg5[i]=NUMBER;
  test_print_all();

  printf("6. Allocate a field of 50 byte: (It should be allocated in Block 5 because of worst fit)\n");
  char* seg6=ts_malloc(50);
  for(int i=0;i<50;i++) seg6[i]=NUMBER;
  test_print_all();

  printf("7. Free seg0: \n");
  ts_free(seg0);
  test_print_all();

  printf("8. Free seg1: (Block 0 and 1 should be merged)\n");
  ts_free(seg1);
  test_print_all();

  printf("8. Free seg6: (Block 4 and 5 should be merged)\n");
  ts_free(seg6);
  test_print_all();

  printf("9. Free seg5: (Block 0, 1 and 2 should be merged)\n");
  ts_free(seg5);
  test_print_all();

  printf("10. Free seg3: (Block 0, 1 and 2 should be merged)\n");
  ts_free(seg3);
  test_print_all();
}

void simple_thread_test_case_2(){
  printf("Test Case 2:\n");
  printf("1. Allocate a field of 1023 byte: \n");
  char* seg0=ts_malloc(1023);
  for(int i=0;i<1023;i++) seg0[i]=NUMBER;
  test_print_all();

  printf("2. Free seg0: \n");
  ts_free(seg0);
  test_print_all();
}

void simple_thread_test_case_3(){
  printf("Test Case 3:\n");
  printf("1. Allocate a field of 990 byte: \n");
  char* seg0=ts_malloc(990);
  for(int i=0;i<990;i++) seg0[i]=NUMBER;
  test_print_all();

  printf("2. Free seg0: \n");
  ts_free(seg0);
  test_print_all();
}

void *alloc_free_1(void *arg){
  char *segs[3];
  for(int i=0;i<3;i++)
    segs[i]=ts_malloc(50+50*i);

  printf("%lld: \n", (unsigned long long)pthread_self());
  test_print_all();
  sleep(1);

  for(int i=0;i<3;i++)
    ts_free(segs[i]);
}

void *alloc_free_2(void *arg){
  char *segs[3];
  for(int i=0;i<3;i++)
    segs[i]=ts_malloc(50+50*i);

  sleep(1);
  printf("%d: \n", (int)pthread_self());
  test_print_all();

  for(int i=0;i<3;i++)
    ts_free(segs[i]);
}

void *alloc_free(void *arg){
  char *segs[10];
  for(int i=0;i<10;i++)
    segs[i]=ts_malloc(rand()%1000);

  sleep(1);

  for(int i=0;i<10;i++){
    ts_free(segs[i]);
  }
}

void multi_thread_test_case_1(){
  printf("Test Case 1./m:\n");
  pthread_t t1, t2;
  pthread_create(&t1, NULL, &alloc_free_1, NULL);
  pthread_create(&t2, NULL, &alloc_free_2, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  test_print_all();
}

void multi_thread_test_case_2(){
  printf("Test Case 2:\n");
  int size=5;
  pthread_t T[size];
  for(int i=0;i<size;i++){
    pthread_create(&T[i], NULL, &alloc_free, NULL);
  }

  for(int i=0;i<size;i++){
    pthread_join(T[i], NULL);
  }

  test_print_all();
}

int main(int argc, char** argv){
  if(argc!=2 || strlen(argv[1])!=2) return 0;
  srand((unsigned int)time(NULL));

  printf("Test system started\n");
  if(argv[1][0]=='s')
    switch(argv[1][1]){
      case '1': simple_thread_test_case_1(); break;
      case '2': simple_thread_test_case_2(); break;
      case '3': simple_thread_test_case_3(); break;
    }
  else
    switch(argv[1][1]){
      case '1': multi_thread_test_case_1(); break;
      case '2': multi_thread_test_case_2(); break;
    }
  return 0;
}