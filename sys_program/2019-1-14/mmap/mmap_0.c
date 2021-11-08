#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
	int ret_op;	//保存open函数的返回值
	int ret_fs;		//保存stat的返回值
	pid_t pid_0;	//保存fork的返回值
	void * ret_mm;	//保存mmap的返回值
	struct stat map_file_stat;	//映射文件的属性
	
	/* 打开（创建）映射文件 */
	ret_op = open("map_file",O_RDWR | O_CREAT,0666);
	if(ret_op < 0)
	{
		perror("open map_file error!");
		return -1;
	}
	
	/* 获取文件属性 */
	ret_fs = fstat(ret_op,&map_file_stat);
	if(ret_fs < 0)
	{	
		perror("fstat map_file error!");
	}
	
	/* 创建子进程 */
	pid_0 = fork();
	if(pid_0 < 0)
	{
		printf("%s:%s:fork error!",__func__,__LINE__);
	}
	else if(pid_0 == 0)
	{
		/* 将文件映射到文件中，实现共享空间 */
		ret_mm = mmap(NULL,map_file_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,ret_op,0);
		if((int)ret_mm == -1)
		{
			perror("mmap map_file error!");
			return -1;
		}
		
		//printf("child:map_file bytes:%d\r\n",map_file_stat.st_size);		//打印文件长度
		
		while(1)
		{
			if(*(char *)ret_mm == 10)
			{	
				printf("-----son process:receive the effective data\r\n");
				*(char *)ret_mm = 1;	//写映射空间
			}
		}
	}
	else
	{	
		char write_data;	//要写入到内存映射区的数据
		/* 将文件映射到文件中，实现共享空间 */
		ret_mm = mmap(NULL,map_file_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,ret_op,0);
		if((int)ret_mm == -1)
		{
			perror("mmap map_file error!");
			return -1;
		}
		
		//printf("parent:map_file bytes:%d\r\n",map_file_stat.st_size);		//打印文件长度
		
		while(1)
		{
			printf("-----parent process:please input write data to map_file：\r\n");
			scanf("%d",&write_data);
			*(char *)ret_mm = write_data;	//写映射空间
			printf("已写入数据：%d\r\n",*(char *)ret_mm);
			sleep(1);
		}
		
		wait(NULL);
	}
	
}








