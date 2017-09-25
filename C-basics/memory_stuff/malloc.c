#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	char *buffer;
	buffer = (char*) malloc(10);
	// if(buffer==NULL)
	// 	exit(1);


	strcpy(buffer, "IamATree");
	printf("%s\n", buffer);
	free(buffer);
	return 0;
}