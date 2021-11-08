#if 0
#include<unistd.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

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
		//sleep(1);//若在此处加延时，将会产生僵尸进程
		printf("i'm parent process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	
	return 0;
}

/*
运行结果：
i'm parent process!
pid = 12640,ppid = 31801
[root@localhost fork]# i'm child process!
pid = 12641,ppid = 1
^C
[root@localhost fork]# 
*/
#endif

#include<unistd.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

//孤儿进程：父进程先消亡，子进程会被init进程回收，ppid变为1
int main()
{
	pid_t tpid = fork();
    fork();
    fork();
    fork();
	/*
	if(tpid < 0)//创建进程失败
	{
		perror("fork is error!");
	}
	else if(tpid == 0)//子进程在运行
	{
		// 子进程在睡眠的过程中，父进程已经消亡，然后init进程将其回收，作为它的父进程 
		sleep(1);
		printf("i'm child process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		//sleep(1);//若在此处加延时，将会产生僵尸进程
		printf("i'm parent process!\r\n");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	*/
    printf("12\n\n");
    while(1);
	return 0;
}

