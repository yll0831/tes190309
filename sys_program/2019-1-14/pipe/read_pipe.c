#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>



int main()
{
	int ret_mk;		//保存 mkfifo 的返回值
	int fd;		//保存 open 的返回值
	char buf_a[20] = "0";		//存取字符串
	ssize_t ret_wr;	//存取write的的返回值
	
	/* 创建管道文件 */
	ret_mk = mkfifo("/fifo_a",0666);
	printf("%d\r\n",errno);
	perror("mk");
	if(errno == EEXIST || ret_mk == 0)	//已经存在或者创建成功
	{
		printf("pipe create success!\r\n");
	}
	else
	{
		printf("pipe create error!\r\n");
		return -1;
	}
	
	/* 打开管道文件 */
	fd = open("/fifo_a",O_RDONLY);
	if(fd < 0)
	{
		printf("open error\r\n");
		return -1;
	}
	
	
	/* 从管道中读数据 */
	ret_wr = read(fd,buf_a,sizeof(buf_a));	
	if(ret_wr < 0)
	{
		printf("read pipe error!\r\n");
	}
	else
	{
		printf("read pipe success:%s\r\n",buf_a);
	}
	
	return 0;
}


