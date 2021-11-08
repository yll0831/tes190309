#include <stdio.h>
#include <unistd.h>

//在一个程序中启动另一个程序
int main()
{
	execlp("pwd","pwd",NULL);
	//execlp("ls","ls","-l",NULL);
	return 0;
}
