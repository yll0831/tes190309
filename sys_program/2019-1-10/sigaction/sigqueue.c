#include <stdio.h>
#include <signal.h>
#include <string.h>



//信号处理函数
void Soft_INT(int sig , siginfo_t * arg , void * data)
{
	printf("NO.%d signal. ",sig);
	//printf("receive:%s\r\n",arg->si_value);
    printf("receive:%s\r\n",arg->si_int);
    
}


//向信号处理函数发送信息只能用新版的信号处理函数（旧版无此功能）
int main()
{
	char TX_BUF[] = "hei!";
	union sigval sig;
	
	sig.sival_ptr = TX_BUF;	//需要发送到信号处理函数的内容
	
	struct sigaction act;
	
	act.sa_sigaction = Soft_INT;	//指定新信号处理函数指针的值
	act.sa_flags = SA_SIGINFO;		//要使用那新版的的信号处理函数，则此项需包含该掩码
	
	sigaction(2,&act,NULL);	//开始捕捉信号
	
	while(1)
	{
		sigqueue(getpid(),2,sig);
		printf("normal run\r\n");
	
		sleep(1);
	}
	
	return 0;

}
/*
运行结果：
NO.2 signal. receive:hei!
normal run
NO.2 signal. receive:hei!
normal run
NO.2 signal. receive:hei!
normal run
NO.2 signal. receive:hei!
normal run
NO.2 signal. receive:hei!
normal run
^\退出 (core dumped)
*/