#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/* 实现两个陌生进程通过内存映射的方式通信 */
int main()
{
	int ret_op;	//保存open函数的返回值
	void * ret_mm;	//保存mmap的返回值
	int ret_fs;		//保存stat的返回值
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
	
	
	printf("map_file bytes:%d\r\n",map_file_stat.st_size);		//打印文件长度
	
	/* 将文件映射到文件中，实现共享空间 */
	ret_mm = mmap(NULL,map_file_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,ret_op,0);
	if((int)ret_mm == -1)
	{
		perror("mmap map_file error!");
		return -1;
	}
	
	printf("the first byte:%d\r\n",*(char *)ret_mm);
	while(1)
	{	
		sleep(1);
		printf("the first byte:%d\r\n",*(char *)ret_mm);
		if(*(char *)ret_mm == 10)
		{
			printf("receive the effective data!\r\n");
			*(char *)ret_mm = 1;	//写映射空间
		}
	}
	
	/* 取消映射 */
	munmap(ret_mm,100);
	
	/* 关闭文件 */
	close(ret_op);
	
}
