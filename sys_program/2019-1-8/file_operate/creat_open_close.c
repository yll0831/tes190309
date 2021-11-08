#include<stdio.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

/*================= 文件操作 creat open close ==================*/
int main()
{
	/* 创建文件并打开？ */
	int ret_c = creat("2.data",S_IRWXU | S_IRWXG | S_IRWXO);
	/* 判断文件是否创建成功 */
	if(ret_c < 0)
	{
		perror("creat is error!");
		return -1;
	}
	printf("ret_c = %d\r\n",ret_c);
	
	/* 关闭文件 */
	close(ret_c);
	
	/* 打开文件 */
	//O_EXCL 与 O_CREAT 连用 如果文件存在则报错，确保不与现存文件冲突，必须与 O_CREAT 连用，否则不起作用
	int ret_o = open("2.data",O_EXCL | O_CREAT,0666);	
	/* 判断是否打开成功 */
	if(ret_o < 0)
	{
		perror("open 2.data error!");
		return -1;
	}
	printf("ret_o = %d\r\n",ret_o);
	
	/* 关闭文件 */
	close(ret_o);

	return 0;
}
