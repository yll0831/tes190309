#include<unistd.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

//僵尸进程
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
		printf("Child process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		sleep(5);		//在睡眠期间，子进程已经消亡，但是其退出状态尚未被回收，此时的子进程称为僵尸进程
		printf("Parent process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	
	return 0;
}
