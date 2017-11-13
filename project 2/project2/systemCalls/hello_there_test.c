#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
int main()
{
	long int amma= syscall(318,"user");
	printf("System call sys_hello_there returned %ld\n",amma);
	return 0;
}
