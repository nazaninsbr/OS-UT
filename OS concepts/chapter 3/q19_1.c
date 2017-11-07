#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
int main(){
	int i;
	int status;
	for(i=0; i<3; i++){
		if(fork()==0){
			printf("child with pid = %d sees i = %d\n", getpid(), i);
		}
		else{
			wait(&status);
			printf("parent with pid = %d sees i = %d\n", getpid(), i);
		}
	}
}
