#include <stdio.h>
#include <unistd.h>

//第一个参数为目标文件，以数组的形式
//在一个程序中运行另一个程序
int main()
{
	//int execvp( const char *file, char *const argv[]);
	
	char * const argv[] = {"ls","-l",NULL};
	
	execvp("ls",argv);
	
	return 0;
}
