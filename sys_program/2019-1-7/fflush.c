#include <stdio.h>

//程序运行结束的时候，系统会自动刷新缓冲区 

int main()
{
	printf("hello world\r\n");
	fflush(NULL);
	while(1)
	{
		
	}
	return 0;
}  
