#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <errno.h>
#include <time.h>
#include <wait.h>

#define SHARED_MEMORY_SIZE 1024

#define MYSEM_INIT 333
#define MYSEM_DOWN 334
#define MYSEM_UP 335

#define MIN_PRIORITY_RANGE -19
#define MAX_PRIORITY_RANGE 20

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))


// shared memory stuff
key_t key;  
int shmid;
char *data;
void* mysem;


int get_priority()
{
	return getpriority(PRIO_PROCESS, getpid());
}

void set_priority(int new_prio) {
	setpriority(PRIO_PROCESS, getpid(), new_prio);
}

void add_priority(int k)
{
	int p = MIN(get_priority() + k, MAX_PRIORITY_RANGE);
	set_priority(p);
}


int main()
{  
	// shared memory stuff
	key = ftok( "/home/shayan/AsA" , 'R'); // 'R' is id not read permission
	shmid = shmget(key, SHARED_MEMORY_SIZE, 0777 | IPC_CREAT);
	data = shmat(shmid, (void *)0 , 0);
	if(data == (char *)(-1)){
		perror( "shmat error.\n" );
		exit(0);
	}
	mysem = data;
 

 	int n = 5, result;
 	// result = mysem_init(mysem, n);
    result = syscall(MYSEM_INIT, mysem, n);

    pid_t  par_pid, chi_pid;
    int i;

    par_pid = getpid();
    printf("before: parrent(%d) prio is: %d\n", par_pid, get_priority());
    add_priority(5);
    printf("after: parrent(%d) prio is: %d\n\n", par_pid, get_priority());


    for (i = 0; i < 10; i++) {
    	if (par_pid != getpid())
    	{
    		continue;
    	}
    	chi_pid = fork();
    	if (chi_pid == 0) {
    		chi_pid = getpid();
    		printf("before: child(%d) prio is: %d\n", chi_pid, get_priority());
    		add_priority(i + 1);
    		printf("after: child(%d) prio is: %d\n\n", chi_pid, get_priority());

    	}

    }

    printf("pid: %d\tprio: %d\n", getpid(), get_priority());

    //result = mysem_down(mysem);
    result = syscall(MYSEM_DOWN, mysem);
    
    printf("I GET THE LOCK! pid: %d\tprio: %d\n", getpid(), get_priority());

    sleep(1);

    //result = mysem_up(mysem);
    result = syscall(MYSEM_UP, mysem);

    printf("I LOOSE THE LOCK! pid %d\tprio: %d\n", getpid(), get_priority());
          
    return 0;

}