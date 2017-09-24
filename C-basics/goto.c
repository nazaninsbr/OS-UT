#include <stdio.h>


int main(){
	int x=9; 

	start:{
		if(x%3==0)
			goto outside;
		goto start;
	}
	outside: printf("you are outside\n");
}