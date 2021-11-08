#include <stdio.h>
#include <signal.h>
#include <string.h>

//新信号处理函数
void Soft_INT(int sig , siginfo_t * arg , void * data)
{
	printf("NO.%d signal：\r\n",sig);	//打印所捕捉到的信号值
	//printf("receive:%d %s\r\n",arg->si_int,arg->si_ptr);	//打印 sigqueue 发送到当前进程的数据
	printf("receive:%d\r\n",arg->si_int);	//打印 sigqueue 发送到当前进程的数据
}

int main()
{
	//定义信号捕捉函数相关结构体变量
	struct sigaction act;
	
	act.sa_sigaction = Soft_INT;	//指定新信号处理函数指针的值
	act.sa_flags = SA_SIGINFO;	//使用新信号处理函数时，需包含此参数
	
	sigaction(2,&act,NULL);	//捕捉 2 信号，及转向信号处理函数处执行
	
	while(1)
	{
		printf("wait for sigqueue...\r\n");
		sleep(1);
	}
	
	return 0;
}
