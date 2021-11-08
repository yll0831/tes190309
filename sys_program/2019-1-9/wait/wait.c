#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


//阻塞模式回收子进程的的退出状态，避免产生孤儿进程
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
		sleep(1);
		printf("i'm child process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		printf("i'm parent process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
		printf("wait to callback the child process...\r\n");
		pid_t ret = wait(NULL);//返回子进程的pid
		printf("ret = %d\r\n",ret);
	}
	
	return 0;
}
/*运行结果
i'm parent process!
pid = 12879,ppid = 31801
wait to callback the child process...
i'm child process!
pid = 12880,ppid = 12879
ret = 12880
*/