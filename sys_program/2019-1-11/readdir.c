#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


int main()
{
	int i;
	struct dirent * attri;	//定义文件属性结构体指针
	
	DIR * ret = opendir("data.txt");	//打开目录
	if(!ret)
	{
		perror("opendir error!\r\n");
	}
		
	for(i = 0;i < 2;i++)
	{
		attri = readdir(ret);	//逐个读取文件信息
		if(attri)
		{
			printf("%s\r\n",attri->d_name);
		}
	}
	
	return 0;
}
