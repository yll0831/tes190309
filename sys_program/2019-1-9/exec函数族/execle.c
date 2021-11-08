#include <stdio.h>
#include <unistd.h>

//extern char **environ;
int main()
{
/*
	int i;
	char * st[3];
	char a[] = "123";
	char b[] = "456";
	char c[] = "789";
	st[0] = a;
		st[1] = b;
			st[2] = c;
	char ** str = st;
	printf("%d\r\n",*str[0]);
	printf("%d\r\n",*str[1]);
*/
	//int execle( const char *path, const char *arg , ..., char * const envp[]);
	
	char * const envo[] = {"PATH = ./path","PASSWD = 123456",NULL};
	
	execle("/usr/bin/env","env",NULL,envo);
	
	return 0;
}
