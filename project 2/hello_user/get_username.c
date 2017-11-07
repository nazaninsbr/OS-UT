#include <unistd.h>
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
    
	uid_t uid, uid2;/*uid_t is a build in data type, which can hold small numbers*/
    char* username;
    uid=getuid();
    uid2=geteuid();
    username = getlogin();
   	write(1, username, sizeof(username));
    return 0;
}