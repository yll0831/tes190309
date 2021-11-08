#if 0
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void fun(int arg )
{
	printf("%d is come!\r\n",arg);
}

int main(int argc,char *argv[])
{
	
	signal(SIGINT,fun);	//捕捉 SIGINT 信号，进入 fun 信号处理函数
	
	if(strcmp("kill",argv[1]) == 0)	
	{
		kill(getpid(),2);	// 向当前进程发送 2 信号
	}
	else if(strcmp("raise",argv[1]) == 0)
	{
		raise(2);	// 向当前进程发送 2 信号
	}
	
	return 0;
}

#endif

#include <unistd.h>
#include <stdio.h>
#include <signal.h>

//信号处理函数
void  fun(int arg)
{
	int i = 0;
	while(i < 3)
	{
		
		printf("%d=%d\r\n",i,arg);
		//用户所希望执行的动作 
		printf("recv  from    SIGINT \r\n");
		i++;
		sleep(1);
	}
	
}

int main()
{
	//可以捕捉 进程中产生的信号 
	signal(SIGRTMIN + 1,fun);
	signal(SIGRTMIN + 1,fun);

	kill(getpid(),SIGRTMIN + 1);
	while(1)
	{
		printf("&&&&&\r\n");
		sleep(1);
	}
	return 0;
}

