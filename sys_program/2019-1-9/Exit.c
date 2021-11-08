#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	printf("hello!");
	
	//exit(0);	//退出前不清除缓冲区
	_exit(0);	//退出前会清除缓冲区
	while(1);
	
}
