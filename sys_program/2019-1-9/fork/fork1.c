#include<unistd.h>
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>

//验证父子进程空间的相互独立
int main()
{
	int a = 5;
	pid_t tpid = fork();
	
	if(tpid < 0)//创建进程失败
	{
		perror("fork is error!");
	}
	else if(tpid == 0)//子进程在运行
	{
		/* 子进程在睡眠的过程中，父进程已经消亡，然后init进程将其回收，作为它的父进程 */
		sleep(1);
		printf("Child process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
		printf("a = %d\r\n",a);
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		a++;
		printf("Parent process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
		printf("a = %d\r\n",a);
	}
	
	return 0;
}

/*运行结果：
Parent process:pid = 12698,ppid = 31801
a = 6
[root@localhost fork]# Child process:pid = 12699,ppid = 1
a = 5
^C
[root@localhost fork]# 
*/