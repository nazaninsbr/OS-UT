#include <linux/kernel.h>
#include <sys/syscall.h>

int main(){
	long int helloWorld = syscall(354);
	return 0;
}
