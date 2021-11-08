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
	char write_data;	//要写入到内存映射区的数据
	int ret_fs;		//保存stat的返回值
	ssize_t ret_wr;		//保存ret_wr 的返回值
	struct stat map_file_stat;	//映射文件的属性
	char  wr_buf[10] = {1,2,3,4,5,6,7,8,'\r','\n'};
	
	/* 打开（创建）映射文件 */
	ret_op = open("map_file",O_RDWR | O_CREAT,0666);
	if(ret_op < 0)
	{
		perror("open map_file error!");
		return -1;
	}
	
	/* 写文件 */
	ret_wr = write(ret_op,wr_buf,10);
	if(ret_wr < 0)
	{
		perror("write map_file error!");
	}
	
	/* 获取文件属性 */
	ret_fs = fstat(ret_op,&map_file_stat);
	if(ret_fs < 0)
	{	
		perror("fstat map_file error!");
	}
	
	
	printf("map_file bytes:%d\r\n",map_file_stat.st_size);	//打印文件长度
	
	/* 将文件映射到文件中，实现共享空间 */
	ret_mm = mmap(NULL,map_file_stat.st_size,PROT_READ | PROT_WRITE,MAP_SHARED,ret_op,0);
	if((int)ret_mm == -1)
	{
		perror("mmap map_file error!");
		return -1;
	}
	
	while(1)
	{	
		printf("please input write data to map_file：\r\n");
		scanf("%d",&write_data);
		*(char *)ret_mm = write_data;	//写映射空间
		printf("已写入数据：%d\r\n",*(char *)ret_mm);
	}
	
	/* 取消映射 */
	munmap(ret_mm,100);
	
	/* 关闭文件 */
	close(ret_op);
	
}
