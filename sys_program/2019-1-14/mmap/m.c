#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main()
{
	//注意  文件不能为空  
	
	//创建文件
	int fd = open("1.txt",O_RDWR | O_CREAT,0666);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

    ftruncate(fd,10);
	
	pid_t pid = fork();
	if(pid == 0)
	{
		 sleep(1);
		 char* p = mmap(NULL,5, PROT_READ| PROT_WRITE,  MAP_PRIVATE,fd,0);
		//建立映射 关系  当前进程与文件1.txt  的关系 
		//char* p = mmap(NULL,10, PROT_READ| PROT_WRITE,  MAP_SHARED,fd,0);
		if(p == (void *)-1)
		{
			perror("mmap");
			exit(1);
		}
		puts(p);
		
		munmap(p,5);
		
		
	}else if(pid > 0)
	{
		// 利用映射 实现进程间的通信  不能设置  MAP_PRIVATE
		//char* p1 = mmap(NULL,10, PROT_READ| PROT_WRITE,  MAP_PRIVATE,fd,0);
		char* p1 = mmap(NULL,10, PROT_READ| PROT_WRITE,  MAP_SHARED,fd,0);
		if(p1 == (void *)-1)
		{
			perror("mmap1");
			exit(1);
		}
		memcpy(p1,"hello",10);

        wait(NULL);	
	}
	
	return 0;
}
