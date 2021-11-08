#include <stdio.h>
#include <unistd.h>

/*============================ 文件操作 =================*/
int main()
{
	
char buf[6] = {'0'};
	/* 标准输入 */
//	read(STDIN_FILENO,buf,5);
	read(0,buf,5);
//	printf("%s\r\n",buf);

	/* 标准输出 */
	//write(1,buf,5);

	//标准错误
	write(2,buf,5);
	
	return 0;
}
