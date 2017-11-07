#include <linux/kernel.h>
//#include <unistd.h>

asmlinkage long sys_hello(char* name)
{
	printk("Hello %s\n", name);
    //write(1,"\nHello ", 7);
	//write(1, name, sizeof(name));
	//write(1, ".\n", 2);
    return 0;
}