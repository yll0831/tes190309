#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* 向文件写内容，将文件中的内容读到终端 */
int main()
{
	char buf[6] = {"hello"};	//缓冲区内容

	//打开已经存在的文件
	int ret_o = open("1.data",O_RDWR);	//以读写方式打开文件，文件必须存在
	/* 判错处理 */
	if(ret_o < 0)
	{
		perror("open 1.data error!");
		return -1;	
	}
	printf("ret_o = %d\r\n",ret_o);

	//向文件写内容
	write(ret_o,buf,5);

	//将文件的操作位置移动到起始位置
	lseek(ret_o,0,SEEK_SET);

	//清空buf
	memset(buf,0,sizeof(buf));
	
	//将文件的内容读到终端
	read(ret_o,buf,5);
	
	/* 关闭文件 */
	close(ret_o);

	printf("buf:%s\r\n",buf);

	return 0;
}





