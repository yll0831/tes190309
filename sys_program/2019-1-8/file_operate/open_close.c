#include<stdio.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


/*===================== 文件操作 =================*/
int main()
{
	/* 打开文件 */
	//已只读的方式，若不存在则创建，若存在不会清除里面的内容
//	int ret_o = open("1.data",O_RDONLY|O_CREAT,0666);
	int ret_o = open("1.txt",O_RDONLY | O_CREAT,S_IRWXU,S_IRWXG,S_IRWXO);	//
	/* 文件打开失败 */
	if(ret_o < 0)
	{
		perror("open is error!");
		return -1;
	}

	printf("ret_o = %d\r\n",ret_o);
	
	/* 关闭文件 */
	close(ret_o);

	return 0;
}
