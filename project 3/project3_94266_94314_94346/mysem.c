#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/ftrace.h>
#include <linux/syscalls.h>
#include <linux/random.h>

//the structs
struct runningProcesses_node{
	struct task_struct *task;
	int lastPriority; 
	//int changedPriority;
	int changed; //has the priority been changed?
};
struct runningProcesses{
	int size; 
	struct runningProcesses_node *head;
};
struct minHeap_node{     //waiter node
	struct task_struct *task; 
	int priority; 
	int waitTime;
	int state;
};
struct minHeap{
	int size;
	struct minHeap_node *head;
};

struct mysem{
	raw_spinlock_t		lock;
	unsigned int		count;
	struct minHeap wait_list; //to be able to keep the waiters by priority
	struct runningProcesses runnings; //processes that have the resource
};


		//defined stuff

//for the minHeap fuctions and struct 
#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

//defines for the mysem struct 
//the actually init being done, very similar to the init in the kernel code 
#define __MYSEMAPHORE_INITIALIZER(name, n)				\
{									\
	.lock		= __SPIN_LOCK_UNLOCKED((name).lock),		\
	.count		= n,						\
	.wait_list	= minHeap_init((name).wait_list),		\
	.run_list	= runningProcesses_init((name).runnings)	\
}

#define DEFINE_MYSEMAPHORE(name)	\
	struct mysem name = __MYSEMAPHORE_INITIALIZER(name, 1)


		//function declerations 

asmlinkage int mysem_down(struct mysem *instance);
asmlinkage int mysem_up(struct mysem *instance);
asmlinkage int mysem_init(struct mysem *instance, int val);
//
static noinline void mysem__down(struct mysem *instance);
static noinline void mysem__up(struct mysem *instance);
//maxHeap
struct minHeap minHeap_init(int size);
void swap_minHeap_nodes(struct minHeap_node *n1, struct minHeap_node *n2);
void heapify_minHeap(struct minHeap *hp, int i);
void print_wait_queue(struct minHeap *hp); 
void insertNode_minHeap(struct minHeap *hp, struct minHeap_node mhn);
struct minHeap_node deleteNode_minHeap(struct minHeap *hp);
int getMinNode(struct minHeap *hp, int i); 
int is_minHeap_empty(struct minHeap *hp); 
//running ha
struct runningProcesses runningProcesses_init(int size);
void insertProcessNode(struct runningProcesses *rp, struct runningProcesses_node data);
void increasePriority(struct runningProcesses *rp, struct minHeap_node waiter); 
void justDecreasePriority(struct runningProcesses *rp, struct task_struct *current_task, int running_priority); 
void decreasePriority(struct runningProcesses *rp, struct task_struct *current_task, int running_priority);
		
		//function defenitions 

//the important mysem functions (the three mentioned in the lab description)
asmlinkage int mysem_init(struct mysem *instance, int val)
{
	static struct lock_class_key __key;
	*instance = (struct mysem) __MYSEMAPHORE_INITIALIZER(*instance, val);
	lockdep_init_map(&instance->lock.dep_map, "semaphore->lock", &__key, 0);
	return 0; 
}
asmlinkage int mysem_down(struct mysem *instance){
	unsigned long flags;
	struct runningProcesses_node data;
	struct task_struct *curTask = current;

	spin_lock_irqsave(&instance->lock, flags);
	if (likely(instance->count > 0)) {	// if semaphore has still room for another process
		data.task = curTask;
		data.lastPriority = task_nice(curTask); //current priority is also the last priority
		data.changed = 0;
		insertProcessNode((&instance->runnings), data); // new process has the semaphore
		instance->count--;
	}
	else
		mysem__down(instance);
	spin_unlock_irqrestore(&instance->lock, flags);
	return 0;
}
asmlinkage int mysem_up(struct mysem *instance){
	unsigned long flags;
	struct task_struct *task = current;
	int currentPriority;
	spin_lock_irqsave(&instance->lock, flags);
	
	currentPriority = task_nice(task); //process who has called signal, has the changed priority
	decreasePriority((&instance->runnings), task, currentPriority); //return the priority to what it was, decrease in a logical sense 

	if (likely(is_minHeap_empty(&instance->wait_list))) {//if list is empty 
		instance->count++;
	}
	else
		mysem__up(instance); 
	spin_unlock_irqrestore(&instance->lock, flags);
}


//different down functions

static inline int __sched mysem__down_common(struct mysem *instance, long state,
								long timeout)
{
	struct task_struct *task = current;
	struct minHeap_node waiter;
	waiter.task = task;
	waiter.state = 0;
	waiter.priority = task_nice(task);
	insertNode_minHeap(&instance->wait_list, &waiter);
	

	for (;;) {
		if (signal_pending_state(state, task))
			goto interrupted;
		if (unlikely(timeout <= 0))
			goto timed_out;
		__set_task_state(task, state);
		raw_spin_unlock_irq(&instance->lock);
		timeout = schedule_timeout(timeout);
		raw_spin_lock_irq(&instance->lock);
		if (waiter.state)
			return 0;
	}

 timed_out:
	deleteNode_minHeap(instance->wait_list);
	//justDecreasePriority(instance->runnings, task, 0);
	return -ETIME;

 interrupted:
	deleteNode_minHeap(instance->wait_list);
	return -EINTR;
}
static noinline void __sched mysem__down(struct mysem *instance)
{
	//long timeOutValue = 10000; 
	//mysem__down_common(instance, TASK_UNINTERRUPTIBLE, timeOutValue);
	

	mysem__down_common(instance, TASK_UNINTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
}
static noinline void __sched mysem__up(struct mysem *instance)
{
	struct minHeap_node *waiter = deleteNode_minHeap(instance->wait_list);
	if(waiter!=NULL){
		waiter->state =1;
		wake_up_process(waiter->task);
	}
}

//the heap functions
struct minHeap minHeap_init(int size){
    struct minHeap hp ;
    hp.size = 0 ;
    hp.head = NULL ;
    return hp ;
    printk("Created a minHeap!\n");
}
void swap_minHeap_nodes(struct minHeap_node *n1, struct minHeap_node *n2){
    struct minHeap_node temp = *n1 ;
    *n1 = *n2 ;
    *n2 = temp ;
    printk("Swapped two minHeap nodes\n");
}
void heapify_minHeap(struct minHeap *hp, int i){
    int smallest = (LCHILD(i) < hp->size && hp->head[LCHILD(i)].priority < hp->head[i].priority) ? LCHILD(i) : i ;
	if(RCHILD(i) < hp->size && hp->head[RCHILD(i)].priority < hp->head[smallest].priority) {
		smallest = RCHILD(i) ;
	}
	if(smallest != i) {
		swap_minHeap_nodes(&(hp->head[i]), &(hp->head[smallest])) ;
		heapify_minHeap(hp, smallest) ;
	}
}
void print_wait_queue(struct minHeap *hp){
    int i;
    for (i = 0; i < hp->size; ++i)
    {
        printk("my priority is:%d\n",hp->head[i].priority);
    }
}
void insertNode_minHeap(struct minHeap *hp, struct minHeap_node mhn){
     if(hp->size) {
		hp->head = krealloc(hp->head, (hp->size + 1) * sizeof(mhn)) ;
	} 
	else {
		hp->head = kmalloc(sizeof(mhn)) ;
	}
	int i = (hp->size)++ ;
	while(i && mhn.priority < hp->head[PARENT(i)].priority) {
		hp->head[i] = hp->head[PARENT(i)] ;
		i = PARENT(i) ;
	}
	hp->head[i] = mhn ;
}
struct minHeap_node deleteNode_minHeap(struct minHeap *hp){
	struct minHeap_node max_priority;
    max_priority = hp->head[0];
	if(hp->size) {
		printk("Deleting node %d\n\n", hp->head[0].priority) ;
		hp->head[0] = hp->head[--(hp->size)] ;
		hp->head = krealloc(hp->head, hp->size * sizeof(struct minHeap_node)) ;
		heapify_minHeap(hp, 0) ;
	}
	else {
		printk("\nMin Heap is empty!\n") ;
		kfree(hp->head) ;
	}
	return max_priority; 
}
int getMaxNode(struct minHeap *hp, int i){
	if(LCHILD(i) >= hp->size) {
		return hp->head[i].priority ;
	}
	int left = getMaxNode(hp, LCHILD(i)) ;
	int right = getMaxNode(hp, RCHILD(i)) ;
	if(left >= right) {
		return left ;
	} 
	else {
		return right;
	}
}
int is_minHeap_empty(struct minHeap *hp){
	if (hp->size == 0)
        return 1;
    return 0;
}



//running process functions 



struct runningProcesses runningProcesses_init(int size) {
	struct runningProcesses rp;
	rp.size = 0;
	rp.head = NULL;
	return rp;
}
void insertProcessNode(struct runningProcesses *rp, struct runningProcesses_node data){
	int i;
	if(rp->size!=0) 
	{
       rp->head = krealloc(rp->head, (rp->size + 1) * sizeof(struct runningProcesses_node), GFP_KERNEL);
    } 
    else 
    {
       rp->head = kmalloc(sizeof(struct runningProcesses_node), GFP_KERNEL);
    }

    i = (rp->size);
    rp->size = rp->size+1;
    rp->head[i] = data;
}
void randomIncreasePriority(struct runningProcesses *rp, struct minHeap_node waiter) { //rp is like minHeap
	int i;
	do{
		get_random_bytes ( &i, sizeof (i) );
		i = i%(rp->size);
		if(waiter.priority < rp->head[i].lastPriority){
			set_user_nice(rp->head[i].task, waiter.priority);	 //change priority to the greater, set its flag to true
			rp->head[i].changed = 1; 
		}
	}while(!(waiter.priority < rp->head[i].lastPriority));
}
void increasePriority(struct runningProcesses *rp, struct minHeap_node waiter) { //rp is like minHeap
	int i;
	for (i = 0; i < rp->size; i++) {
		if (waiter.priority < rp->head[i].lastPriority) { //priority of waiter is greater than this process in semaphore
			set_user_nice(rp->head[i].task, waiter.priority);	 //change priority to the greater, set its flag to true
			rp->head[i].changed = 1; 
			return; 
		}		
	}
}
void deleteFromRunningList(struct runningProcesses *rp, int pos) {
	int i;
	for(i = pos; i < rp->size - 1; i++) 
		rp->head[i] = rp->head[i + 1];
}
void decreasePriority(struct runningProcesses *rp, struct task_struct *current_task, int running_priority) {
	int i,priority ;
	for (i = 0; i < rp->size; i++) {
		if (rp->head[i].task->pid == current_task->pid && rp->head[i].changed == 1) {
			rp->head[i].changed = 0; 
			priority = rp->head[i].lastPriority;
			set_user_nice(current_task, priority);
			deleteFromRunningList(rp, i);
		}
	}
}
void justDecreasePriority(struct runningProcesses *rp, struct task_struct *current_task, int running_priority) {
	int i,priority ;
	for (i = 0; i < rp->size; i++) {
		if (rp->head[i].task->pid == current_task->pid && rp->head[i].changed == 1) {
			rp->head[i].changed = 0; 
			priority = rp->head[i].lastPriority;
			set_user_nice(current_task, priority);
		}
	}
}
