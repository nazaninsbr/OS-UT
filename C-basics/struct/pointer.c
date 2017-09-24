#include <stdio.h>


struct example{
	int x;
};


int main(){

	struct example an_example;
	struct example *ptr;
	an_example.x = 32;

	ptr = &an_example;
	printf("%d\n", ptr->x);
}