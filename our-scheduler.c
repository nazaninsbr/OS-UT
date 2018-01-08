/* Creates several threads all at the same priority and ensures
   that they consistently run in the same round-robin order.

   Based on a test originally submitted for Stanford's CS 140 in
   winter 1999 by by Matt Franklin
   <startled@leland.stanford.edu>, Greg Hutchins
   <gmh@leland.stanford.edu>, Yu Ping Hu <yph@cs.stanford.edu>.
   Modified by arens. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "devices/timer.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "userprog/syscall.h"
#include "lib/syscall-nr.h"
#include "tests/lib.h"
#include "tests/main.h"

void
test_our_scheduler (void) ;

struct simple_thread_data 
  {
    int id;                     /* Sleeper ID. */
    int iterations;             /* Iterations so far. */
    struct lock *lock;          /* Lock on output. */
    int **op;                   /* Output buffer position. */
  };

#define THREAD_CNT 5
#define ITER_CNT 16

static thread_func simple_thread_func;

void
test_our_scheduler (void) 
{
 /* struct simple_thread_data data[THREAD_CNT];
  struct lock lock;
  int *output, *op;
  int i, cnt;


  msg ("%d threads will iterate %d times in the same order each time.",
       THREAD_CNT, ITER_CNT);
  msg ("If the order varies then there is a bug.");

  output = op = malloc (sizeof *output * THREAD_CNT * ITER_CNT * 2);
  ASSERT (output != NULL);
  lock_init (&lock);

 // syscall_add_device("one");
 // syscall_add_device("two");
 // syscall_add_device("three");

  	thread_set_priority (PRI_DEFAULT + 2);
  	for (i = 0; i < THREAD_CNT; i++) 
  	{
      	char name[16];
      	struct simple_thread_data *d = data + i;
      	snprintf (name, sizeof name, "%d", i);
      	d->id = i;
      	d->iterations = 0;
      	d->lock = &lock;
      	d->op = &op;
      	if(i==0)
      		thread_create (name, PRI_DEFAULT + 1, simple_thread_func, d);
      	if(i==1)
      		thread_create (name, PRI_DEFAULT + 1, thread_func_1, d);
      	if(i==2)
      		thread_create (name, PRI_DEFAULT + 1, thread_func_2, d);
      	if(i==3)
      		thread_create (name, PRI_DEFAULT + 1, thread_func_3, d);
      	if(i==4)
      		thread_create (name, PRI_DEFAULT + 1, thread_func_4, d);
  	}*/
    syscall_print_device_names();

  // thread_set_priority (PRI_DEFAULT);
  // /* All the other threads now run to termination here. */
  // ASSERT (lock.holder == NULL);

  // cnt = 0;
  // for (; output < op; output++) 
  //   {
  //     struct simple_thread_data *d;

  //     ASSERT (*output >= 0 && *output < THREAD_CNT);
  //     d = data + *output;
  //     if (cnt % THREAD_CNT == 0)
  //       printf ("(priority-fifo) iteration:");
  //     printf (" %d", d->id);
  //     if (++cnt % THREAD_CNT == 0)
  //       printf ("\n");
  //     d->iterations++;
  //   }
}

static void 
simple_thread_func (void *data_) 
{
	printf("S0\n");
  	struct simple_thread_data *data = data_;
  	syscall_device_request_at_near_future("A"); 
  	syscall_device_request_at_near_future("B"); 
  	for(int i=0; i<1000; i++);
  	syscall_device_request("A", 12);
  	for(int i=0; i<500; i++);
 	syscall_device_request("B", 100);
 	printf("E0\n");
}
static void thread_func_1(void *data_)
{
	printf("S1\n");
	struct simple_thread_data *data = data_;
	syscall_device_request_at_near_future("A"); 
	for(int i=0; i<100; i++);
	syscall_device_request("A", 12);
	printf("E1\n");
}
static void thread_func_2(void *data_)
{
	printf("S2\n");
	struct simple_thread_data *data = data_;
	syscall_device_request_at_near_future("C"); 
	for(int i=0; i<10000; i++);
	syscall_device_request("C", 12);
	printf("E2\n");
}
static void thread_func_3(void *data_)
{
	printf("S3\n");
	struct simple_thread_data *data = data_;
	syscall_device_request_at_near_future("A"); 
	for(int i=0; i<8000; i++);
	printf("E3\n");
}
static void thread_func_4(void *data_)
{
	printf("S4\n");
	struct simple_thread_data *data = data_;
	syscall_device_request_at_near_future("A"); 
	syscall_device_request_at_near_future("B");
	syscall_device_request_at_near_future("C");
	for(int i=0; i<900; i++);
	syscall_device_request("A", 12);
	for(int i=0; i<900; i++);
	syscall_device_request("C", 12);
	for(int i=0; i<900; i++);
	syscall_device_request("B", 12);
	printf("E4\n");
}