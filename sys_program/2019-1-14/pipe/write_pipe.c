#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


int main()
{
	int ret_mk;		//保存 mkfifo 的返回值
	int fd;		//保存 open 的返回值
	char buf_a[20] = "0";		//存取输入的字符串
	ssize_t ret_wr;	//存取write的的返回值
	
	/* 创建管道文件 */
	ret_mk = mkfifo("/etc/fifo_a",0666);
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
	fd = open("/fifo_a",O_WRONLY);
	if(fd < 0)
	{
		printf("open error\r\n");
		return -1;
	}
	
	
	printf("请输入要发送到管道中的内容:\r\n");
	gets(buf_a);	// 输入字符串
	
	/* 向管道中写数据 */
	ret_wr = write(fd,buf_a,sizeof(buf_a));	
	if(ret_wr < 0)
	{
		printf("write error!\r\n");
	}
	else
	{
		printf("write pipe success!\r\n");
	}
	
	return 0;
}


