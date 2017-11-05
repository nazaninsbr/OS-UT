#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
int main()   
{      
	/*
		Method 1
	*/
	//struct passwd *passwd;
	//passwd = getpwuid ( getuid()); 
	//printf("The Login Name is %s ", passwd->pw_name);
	/*
		Method 2
	*/
	//printf("%s@shell:~%s$", getenv("LOGNAME"), getcwd(currentDirectory, 1024));
    
	uid_t uid;/*uid_t is a build in data type, which can hold small numbers*/
    char* username;
    uid=getuid();
    username=getlogin();
    printf("\nUID is:%d\nUsername:%s\n\n\n",uid,username);
    return 0;
}