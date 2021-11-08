#include <stdio.h>
#include <signal.h>
#include <string.h>

//获取接收方的进程号 --从外部传进来
//将信号发送置接收进程
int main(int argc,char * argv[])
{
	int ret = atoi(argv[1]);	//目标进程pid	
	int sign = atoi(argv[2]);	//待发送的信号
	
	/* 定义 sigqueue 相关结构提变量 */
	union sigval sig;	//存放传送给信号处理函数的数据
	sig.sival_int = 24;
	//sig.sival_ptr = argv[3];
	
	int ret_s;	//存取sigqueue函数的返回值
	
	while(1)
	{
		ret_s = sigqueue(ret,sign,sig);
	
		if(ret_s == 0)
		{
			printf("send success!\r\n");
		}
		ret_s = 1;
		sleep(1);
	
	}
	
	return 0;
	
}
