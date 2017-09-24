#include <stdio.h>


struct example{
	int x;
};


int main(){

	struct example an_example;
	an_example.x = 32;
	printf("%d\n", an_example.x);
}