#include <stdio.h>
#include <unistd.h>

//已数组的形式
//在一个程序中运行另一个程序
int main()
{
	//int execv(const char *path, char *const argv[]);
	
	char * const argv[] = {"ls","-l",NULL};
	
	execv("/bin/ls",argv);
	
	return 0;
}
