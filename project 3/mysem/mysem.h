#ifndef __LINUX_MYSEMAPHORE_H
#define __LINUX_MYSEMAPHORE_H

#include <linux/list.h>
#include <linux/spinlock.h>
#include "linux/max_heap.h"
#include "linux/running_processes.h"

/* Please don't access any members of this structure directly */
struct mysem{
	raw_spinlock_t		lock;
	unsigned int		count;
	struct maxHeap wait_queue; //to be able to keep the waiters by priority
	struct runningProcesses runnings; //processes that have the resource
	//this was what semaphore originally had: struct list_head	wait_list;
};

#define __MYSEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= __SPIN_LOCK_UNLOCKED((name).lock),		\
	.count		= n,						\
	.wait_list	= maxHeap_init((name).wait_list),		\
	.run_list	= runningProcesses_init((name).run_list)	\
}

#define DEFINE_MYSEMAPHORE(name)	\
	struct mysem name = __MYSEMAPHORE_INITIALIZER(name, 1)

static inline void mysem_init(struct mysem *instance, int val)
{
	static struct lock_class_key __key;
	*sem = (struct mysem) __MYSEMAPHORE_INITIALIZER(*sem, val);
	lockdep_init_map(&sem->lock.dep_map, "semaphore->lock", &__key, 0);
}

extern void mysem_down(struct mysem *instance);
extern void mysem_up(struct mysem *instance);

#endif