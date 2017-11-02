#include <linux/kernel.h>

asmlinkage long sys_hello_world(void){
	printk("Hello World!\n");
	return 0;
}
