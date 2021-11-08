#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* 创建两个管道，实现双向通信 */
int main()
{
	int  fd_a[2];		//管道_0
	int  fd_b[2];		//管道_1
	int  ret_p;		//保存 pipe 函数的返回值
	pid_t pid_f;	//保存fork的返回值
		
	char * pbuf_a = "hello world";
	char * pbuf_b = "hei";
	char buf_a[20] = {'0'};
	
	
	/* 创建管道 fd_a */
	ret_p = pipe(fd_a);	
	if(ret_p < 0)
	{
		printf("%s:%s:create pipe fd_a error!\r\n",__func__,__LINE__);
		exit(1);
	}
	
	ret_p = pipe(fd_b);
	if(ret_p < 0)
	{
		printf("%s:%s:create pipe fd_b error!\r\n",__func__,__LINE__);
		exit(1);
	}
	
	/* 创建进程 */
	pid_f = fork();
	if(pid_f < 0)
	{
		printf("%s:%s:fork error!\r\n",__func__,__LINE__);
		exit(1);
	}
	else if(pid_f == 0)
	{
		
		/* 子进程向管道fd_b写端写数据  */
		printf("------------------son write pipe-----------------\r\n");
		write(fd_b[1],pbuf_b,strlen(pbuf_b));
		
		while(1)
		{
			/* 子进程 将管道fd_a  的数据读出 */
			if(read(fd_a[0],buf_a,strlen(pbuf_a)))
			{
				printf("------------------son read pipe:%s\r\n",buf_a);
				break;
			}
		}
		
	}
	else
	{
		/* 父进程向管道fd_a写数据  */
		printf("------------------parent write pipe-----------------\r\n");
		write(fd_a[1],pbuf_a,strlen(pbuf_a));
		
		while(1)
		{
			/* 父进程 将管道fb_a 的数据读出 */
			if(read(fd_b[0],buf_a,strlen(pbuf_b)))
			{
				printf("------------------parent read pipe:%s\r\n",buf_a);
				break;
			}
		}
		wait(NULL);
			
	}
	close(fd_a[0]);
	close(fd_a[1]);
	close(fd_b[0]);
	close(fd_b[1]);
	
	return 0;
}
