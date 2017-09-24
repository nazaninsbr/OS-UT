#include <stdio.h>


int main(){

	int x;
	scanf("%d", &x);
	printf("the number you entered was:%d\n", x);
	if(x<0){
		printf("negative\n");
	}
	else{
		printf("not negative!\n");
	}

	return 0;
}