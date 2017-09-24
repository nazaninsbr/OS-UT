#include <stdio.h>


int main(){

	char c;
	scanf("%c", &c);
	switch(c){
		case 'Y':
			printf("Yes\n");
			break;
		case 'N':
			printf("No\n");
			break;
		default:
			printf("invalid input\n");
			break;
	}
	return 0;
}