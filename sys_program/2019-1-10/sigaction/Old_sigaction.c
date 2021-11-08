#include <stdio.h>
#include <signal.h>
#include <string.h>

//旧信号处理函数
void Soft_INT(int arg)
{
	printf("reveive NO.%d signal!\r\n",arg);
}


int main()
{
	struct sigaction act;

	act.sa_handler = Soft_INT;	//指定旧信号处理函数指针的值
	act.sa_flags = 0;		//要要使用旧版的的信号处理函数，则此项需置0
	
	sigaction(2,&act,NULL);
	while(1)
	{
		printf("normal run\r\n");
	
		sleep(1);
	}
	
	return 0;
}
