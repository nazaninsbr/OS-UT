#include <iostream>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int main(){
	int pid = getpid();
	cout<<"0. I am process "<<getpid()-pid<<endl;
	fork();
	wait(NULL);
	fork();
	wait(NULL);
	cout<<"1. I am process "<<getpid()-pid<<endl;
	fork();
	wait(NULL);
	cout<<"2. I am process "<<getpid()-pid<<endl;
	fork();
	wait(NULL);
	cout<<"3. I am process "<<getpid()-pid<<endl;
	return 0;
}
