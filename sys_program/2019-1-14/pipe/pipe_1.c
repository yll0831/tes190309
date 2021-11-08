#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* 一个管道实现双向通信 */
int main()
{
	
	int  fd[2];		//定义管道变量
	char buf[20] = {'0'};	//定义缓冲区
	int  ret_pi;	//存取pipe的返回值
	pid_t pid_fo;	//存取fork的返回值
	
	/* 创建管道 */
	ret_pi = pipe(fd);
	/* 创建管道失败 */		
	if(ret_pi < 0)
	{
		perror("pipe\r\n");
		exit(1);
	}
	
	
	/* 创建子进程 */
	pid_fo = fork();
	
	if(pid_fo < 0)
	{
		printf("fork is error\r\n");
		exit(1);
	}
	else if(pid_fo == 0)
	{
		while(1)
		{
			memset(buf,0,sizeof(buf));	//清零数组
			sleep(1);	//睡眠，让父进程读走管道中的数据
			/* 读取成功 */
			if(read(fd[0],buf,sizeof(buf)))	
			{
				printf("son receive:%s\r\n",buf);
			}
			printf("向父进程发送信息：\r\n");
			gets(buf);	//输入需要发送的字符串
			write(fd[1],buf,strlen(buf));	//写管道
		}
		
	}
	else
	{
		while(1)
		{
			printf("向子进程发送信息：\r\n");
			gets(buf);	//输入需要发送的字符串
			write(fd[1],buf,strlen(buf));	//写管道
			memset(buf,0,sizeof(buf));	//清零数组
			sleep(1);	//睡眠，让子进程读走管道中的数据
			/* 读取成功 */
			if(read(fd[0],buf,sizeof(buf)))
			{
				printf("parent receive:%s\r\n",buf);
			}
		}
		
		wait(NULL);	//回收子进程
		
			
	}
	close(fd[0]);
	close(fd[1]);
	
	return 0;
}
