#ifndef __RUNNINGPROCESS_H
#define __RUNNINGPROCESS_H

#include <linux/list.h>
#include <linux/sched.h>
#include "linux/min_heap.h"

struct runningProcesses_node{
	struct task_struct *task;
	int lastPriority; 
	int changedPriority;
	int changed; //has the priority been changed?
};
struct runningProcesses{
	int size; 
	struct runningProcesses_node *head;
};

struct runningProcesses runningProcesses_init(int size);

void insertRunningProcess(struct runningProcesses *rp, struct runningProcesses_node data);

void increasePriority(struct runningProcesses *rp, struct maxHeap_node waiter);

void rollbackPriority(struct runningProcesses *rp, struct task_struct *current_task, int running_priority);

#endif