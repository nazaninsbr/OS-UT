#include "threads/synch.h"
#include <list.h>
struct our_device{
	char name[8];
	struct list wait_queue;
	int64_t time_needed; 
	struct lock deviceLock;
	struct lock timeLock;
	
};
/*struct our_device_controller{
	struct our_device this_device;
	int64_t time_needed; 
	struct lock deviceLock;
	struct lock TimeLock;
};*/
/*struct all_our_devices{
	struct list_head all_devices;//change it 
};*/



void add_device(char *name);
void add_device_request(char *name, int64_t duration);
void device_request_at_near_future(char *name);
void our_device_sleep(int sleep_time);
//void wait(struct our_device_controller* device);
//void signal(struct our_device_controller* device);
//void setTime(struct our_device_controller* device);
int64_t getQueueTimeByIndex(int index);
int64_t getQueueTime(char *name);
int device_status(void);
int calculate_less_duration(void);
struct thread* device_running_thread(int stat);
void our_device_signal(int device_num);
void our_reduce_device_duration(int device_num, int64_t passed_duration);
void acquire_device(int device_num);
void get_device_names(void);

