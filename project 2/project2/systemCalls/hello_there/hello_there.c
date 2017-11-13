#include <linux/kernel.h>
 asmlinkage long sys_hello_there(char* name)
{
	printk("Hello %s\n", name);
	return 0;
}
