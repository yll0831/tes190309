#include <stdio.h>
#include <signal.h>
#include <string.h>

//信号处理函数
void Soft_INT(int sig , siginfo_t * arg , void * data)
{
	printf("reveive NO.%d signal!\r\n",sig);
}

int main()
{
	struct sigaction act;
	
	act.sa_sigaction = Soft_INT;	//指定新信号处理函数指针的值
	act.sa_flags = SA_SIGINFO;		//要使用那新版的的信号处理函数，则此项需包含改掩码
	
	sigaction(2,&act,NULL);		//开始捕捉信号
	while(1)
	{
		printf("normal run\r\n");
	
		sleep(1);
	}
	
	return 0;

}
