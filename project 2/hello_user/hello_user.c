#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/kernel.h>

asmlinkage long sys_hello_user(void){
	uid_t uid;/*uid_t is a build in data type, which can hold small numbers*/
    char* username;
    uid=getuid();
    username=getlogin();
	printk("Hello %s\n", username);
	write(1,"\nHello ", 7);
	write(1, username, sizeof(username));
	write(1, ".\n", 2);
	return 0;
}
