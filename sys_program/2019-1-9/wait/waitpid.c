#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//非阻塞模式回收子进程的的退出状态，避免产生孤儿进程
//孤儿进程：父进程先消亡，子进程会被init进程回收，ppid变为1
int main()
{
	pid_t tpid = fork();
	
	if(tpid < 0)//创建进程失败
	{
		perror("fork is error!");
	}
	else if(tpid == 0)//子进程在运行
	{
		/* 子进程在睡眠的过程中，父进程已经消亡，然后init进程将其回收，作为它的父进程 */
		sleep(5);
		printf("i'm child process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		printf("i'm parent process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
		printf("wait to callback the child process...\r\n");
		while(1)
		{
			pid_t ret = waitpid(-1,NULL,WNOHANG);//返回子进程的pid
			if(ret != 0)
			{
				printf("\r\nret = %d\r\n",ret);
				break;
			}
		}
	}
	
	return 0;
}
