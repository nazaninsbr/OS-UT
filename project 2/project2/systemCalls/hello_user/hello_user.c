#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cred.h>
#include <linux/sched.h>
//#include <unistd.h>
 asmlinkage long sys_hello_user(void)
{
	//char* username;
	//username=getlogin();
	printk("Hello %u\n", get_current_user()->uid);
	return 0;
}
