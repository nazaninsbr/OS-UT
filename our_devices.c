#include <list.h>
#include <string.h>
#include "devices/timer.h"
#include "devices/our_devices.h"
#include "threads/thread.h"

#define NUM_OF_DEVICES 3
struct our_device all_our_devices[NUM_OF_DEVICES];
int j=0;  

/*
 struct list_elem *it = list_front(&blocked_sleeping_list);
    struct thread *t = list_entry(it, struct thread, elem);*/                                  
void our_device_sleep(int sleep_time)
{	int i;
	for(i=0;i<1000*sleep_time;i++);
}
void add_device(char *name){
	
	int i;
	for(i=0;i<NUM_OF_DEVICES;i++)
	{
		if(strcmp(all_our_devices[i].name,name)==1)    
			return;
	}

	strlcpy(all_our_devices[j].name,name,sizeof(name));
	list_init(&all_our_devices[j].wait_queue);
	all_our_devices[j].time_needed=0;
	lock_init(&all_our_devices[j].timeLock);
	lock_init(&all_our_devices[j].deviceLock);
	j++;
	j=j%3;
}
void device_request_at_near_future(char *name)
{
	struct thread *cur=running_thread();
	int i;
	if(cur->initAskValues == 1){
		for(int i=0; i<NUM_OF_DEVICES; i++)
			cur->ask_device[i]=0;
		cur->initAskValues = 0;
	}
	for(i=0;i<NUM_OF_DEVICES;i++){
		if(strcmp(all_our_devices[i].name,name)==1)
		{
			cur->ask_device[i]=1;
		}
	}
}

void add_device_request(char *name, int64_t duration){
	
	struct thread *cur = running_thread ();
	int i;
	int index = -1;
	for(i=0;i<NUM_OF_DEVICES;i++)
	{
		if(strcmp(all_our_devices[i].name,name)==1)
		{
			
			lock_acquire(&all_our_devices[i].timeLock);
			index = i;
			all_our_devices[i].time_needed +=duration;
			list_push_back(&(all_our_devices[i].wait_queue),(struct list_elem*) cur);
			cur->blocked.io_duration[i] = duration;
			lock_release(&all_our_devices[i].timeLock);
		}
    }
    if(index!=-1){
    	struct thread *cur = running_thread ();
		cur->status = THREAD_BLOCKED;
		cur->blocked.reason = WAITING;
    	timer_msleep(all_our_devices[index].time_needed-duration);
    	acquire_device(index);
    	//lock_acquire(&all_our_devices[index].timeLock);
		//all_our_devices[i].time_needed -=duration;
		//remove from list
		//list_pop_front(&(all_our_devices[index].wait_queue));
		//cur->blocked.io_duration[index] = 0;
		//lock_release(&all_our_devices[index].timeLock);
		cur->status = THREAD_READY;
    }
}

int device_status(void)
{
	int i;
	int cnt=0;
	int cnt2 = 0;
	for(i=0;i<NUM_OF_DEVICES;i++)
	{
		if(list_empty(&all_our_devices[i].wait_queue))
		{
			cnt++;
		}
	}
	for(i=0;i<NUM_OF_DEVICES;i++)
	{
		if(our_lock_try_acquire(&all_our_devices[i].deviceLock)==false)
		{
			cnt2++;
		}
	}
	if(cnt==NUM_OF_DEVICES)
	   return -1;
	if(cnt2==NUM_OF_DEVICES)
	   return -1;
	else
	{
		return calculate_less_duration();	
	}
	
}

int calculate_less_duration(void)
{ 
	if(our_lock_try_acquire(&all_our_devices[0].deviceLock)==false && our_lock_try_acquire(&all_our_devices[1].deviceLock)==false)
		return 2;
	if(our_lock_try_acquire(&all_our_devices[0].deviceLock)==false && our_lock_try_acquire(&all_our_devices[2].deviceLock)==false)
		return 1;
	if(our_lock_try_acquire(&all_our_devices[1].deviceLock)==false && our_lock_try_acquire(&all_our_devices[2].deviceLock)==false)
		return 0;
	if(our_lock_try_acquire(&all_our_devices[0].deviceLock)==false){
		if(all_our_devices[1].time_needed<all_our_devices[2].time_needed)
			return 1;
		else 
			return 2;
	}
	if(our_lock_try_acquire(&all_our_devices[1].deviceLock)==false){
		if(all_our_devices[0].time_needed<all_our_devices[2].time_needed)
			return 0;
		else 
			return 2;
	}
	if(our_lock_try_acquire(&all_our_devices[2].deviceLock)==false){
		if(all_our_devices[1].time_needed<all_our_devices[0].time_needed)
			return 1;
		else 
			return 0;
	}
	else{
		if(all_our_devices[0].time_needed<all_our_devices[1].time_needed && all_our_devices[0].time_needed<all_our_devices[2].time_needed)
			return 0;
		else if(all_our_devices[1].time_needed<all_our_devices[0].time_needed && all_our_devices[1].time_needed<all_our_devices[2].time_needed)
			return 1;
		else if(all_our_devices[2].time_needed<all_our_devices[0].time_needed && all_our_devices[2].time_needed<all_our_devices[1].time_needed)
			return 2;
		else return 0;
	}
	
	return 0;
}

struct thread* device_running_thread(int stat)
{
	ASSERT(stat < NUM_OF_DEVICES);
	if(list_empty(&all_our_devices[stat].wait_queue))
		return NULL;
	struct list_elem *it = list_begin(&all_our_devices[stat].wait_queue);
    	struct thread *t = list_entry(it, struct thread, elem); 
		return t;
}

// void wait(struct our_device* device){	
// 	lock_acquire (&(device->deviceLock));
// }
// void signal(struct our_device* device){
// 	lock_release (&(device->deviceLock));
// }
/*void setTime(struct our_device* device){
	int64_t sum=0;
	for(int i=0;i<list_size(&(this_device.wait_queue));i++)        ///shayad lazem she!
		sum+=device->this_device.wait_queue[i].time;
	device->time_needed=sum;
}*/
int64_t getQueueTimeByIndex(int index){
	if(index<NUM_OF_DEVICES)
		 	return all_our_devices[index].time_needed;
	else
		return 0;
}
int64_t getQueueTime(char *name){
	
	int i;
	for(i=0;i<NUM_OF_DEVICES;i++){
		 if(strcmp(all_our_devices[i].name,name)==1)
		 {
		 	return all_our_devices[i].time_needed;
		 }
	}
	return 0;
}
void our_device_signal(int device_num){
	lock_release (&(all_our_devices[device_num].deviceLock));
	list_pop_front(&(all_our_devices[device_num].wait_queue));
}
void our_reduce_device_duration(int device_num, int64_t passed_duration){
	all_our_devices[device_num].time_needed -= passed_duration;
}
void acquire_device(int device_num){
	if(! lock_held_by_current_thread(&(all_our_devices[device_num].deviceLock)))
	 	lock_acquire(&(all_our_devices[device_num].deviceLock));

	struct thread *cur = running_thread ();
	timer_msleep(cur->blocked.io_duration[device_num]);
	our_device_signal(device_num);
	our_reduce_device_duration(device_num, cur->blocked.io_duration[device_num]);
	cur->blocked.io_duration[device_num] = 0;
}
void get_device_names(void)
{
	int i;
	for(i=0;i<NUM_OF_DEVICES;i++)
	printf("The name of device is %s\n",all_our_devices[i].name);
}



