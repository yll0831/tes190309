#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//进程与文件的结合
int main()
{
	char buf1[20] = {"he!"};
	char buf2[20] = {'0'};
	
	//打开文件
	int fd = open("1.data",O_RDWR | O_CREAT,0666);	
	
	write(fd,buf1,3);
	
	
	if(fd < 0)
	{
		printf("open is error!");
		return -1;
	}
	
	
	
	//创建子进程
	pid_t tpid = fork();
	
	if(tpid < 0)//创建进程失败
	{
		perror("fork is error!");
	}
	else if(tpid == 0)//子进程在运行
	{
		printf("Child process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
		lseek(fd,0,SEEK_SET);
		read(fd,buf2,sizeof(buf2));
		printf("%s\r\n",buf2);
	}
	else//父进程在运行,返回的是子进程的的pid
	{
		printf("Parent process:");
		printf("pid = %d,ppid = %d\r\n",getpid(),getppid());
	}
	
	return 0;
}
