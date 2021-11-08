#include <stdio.h>
#include <sys//types.h>
#include <sys/stat.h>
//#include<fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/types.h>
#include <dirent.h>
#include <grp.h>

/* 实现 ls 的功能 opendir readdir */
int main(int argc,char * argv[])
{
	DIR * DIR_0; //存取 opendir 的返回值
	struct dirent * dirent_0;	//保存 readdir 的返回值
	struct stat stat_0;	//stat 函数的第二个参数，存取 stat 函数 提取的文件属性
	int ret_st;	//存取 stat 函数的返回值
	struct passwd * passwd_0;	//获取存取用户信息，
	struct group * group_0;	//存取用户组信息
	char * s_time; 		//存取时间字符串地址
	
	/* 参数个数判错 */
	if(argc != 2)
	{
		printf("only allow 2 arg!");
		return -1;
	}
	
	/* 打开目录 */
	DIR_0 = opendir(argv[1]);
	/* 判错处理 */
	if(DIR_0 == NULL)
	{
		perror("opendir:");
		return -1;
	}
	while(1)
	{
		/* 逐个获取指定目录下文件的简略属性 */
		dirent_0 = readdir(DIR_0);
		
		/* 获取文件属性失败 */
		if((int)dirent_0 == -1)
		{
			perror("readdir error!");
			return -1;
		}
		/* 当前目录下的文件名已经读取完毕 */
		else if((int)dirent_0 == 0)
		{
			break ;
		}
		
		/* 排除第一个.文件 */
		if('.' == *dirent_0->d_name)
		{
			continue;	
		}
		else
		{
			memset(&stat_0,0,sizeof(stat_0));
			
			/* 读取文件的详细属性 */
			ret_st = stat(dirent_0->d_name,&stat_0);
			/* 读取错误 */
			if(ret_st < 0)
			{
				printf("%s\r\n","111");
				perror("aaastat error!");
				return -1;
			}
			
			/* 打印文件类型 */
			/* 将 stat_0.st_mode 与上文件类型位遮罩 S_IFMT */
			switch(stat_0.st_mode & S_IFMT)
			{
				case S_IFREG:
					printf("-");	//普通文件
					break;
				case S_IFBLK:
					printf("b");	//快设备
					break;
				case S_IFCHR:
					printf("c");	//字符类型			
					break;
				case S_IFDIR:		//设备类型
					printf("d");
					break;
				case S_IFLNK:		//链接类型
					printf("l");
					break;
				case S_IFIFO:		//管道类型
					printf("p");
					break;	
				case S_IFSOCK:		//socket类型
					printf("s");
					break;					
			}
			
			/* 打印文件所有者权限 */
			/* S_IRWXU   00700文件所有者权限  */
			switch(stat_0.st_mode & S_IRWXU)
			{
				case 0:
					printf("---");
					break;
				case 0100:
					printf("--x");
					break;
				case 0200:
					printf("-w-");
					break;
				case 0300:
					printf("-wx");
					break;
				case 0400:
					printf("r--");
					break;
				case 0500:
					printf("r-x");
					break;
				case 0600:
					printf("rw-");
					break;
				case 0700:
					printf("rwx");
					break;
			}
			
			/* 打印文件所属组的权限 */
			switch(stat_0.st_mode & S_IRWXG )
			{
				case 0:
					printf("---");
					break;
				case 010:
					printf("--x");
					break;
				case 020:
					printf("-w-");
					break;
				case 030:
					printf("-wx");
					break;
				case 040:
					printf("r--");
					break;
				case 050:
					printf("r-x");
					break;
				case 060:
					printf("rw-");
					break;
				case 070:
					printf("rwx");
					break;
			}
			/* 文件其他组权限 */
			switch(stat_0.st_mode & S_IRWXO )
			{
				case 0:
					printf("---");
					break;
				case 01:
					printf("--x");
					break;
				case 02:
					printf("-w-");
					break;
				case 03:
					printf("-wx");
					break;
				case 04:
					printf("r--");
					break;
				case 05:
					printf("r-x");
					break;
				case 06:
					printf("rw-");
					break;
				case 07:
					printf("rwx");
					break;
			}
			
			printf(". ");
			
			/* 打印文件链接数 */	
			printf("%d",stat_0.st_nlink);
			
			printf(" ");
			
			/* 打印文件所有者 */
			passwd_0 = getpwuid(stat_0.st_uid);
			printf("%s",passwd_0->pw_name);
			
			printf(" ");
		
			/* 打印文件所属组 */
			group_0 = getgrgid(stat_0.st_gid);
			printf("%s",group_0->gr_name);
			
			printf(" ");
			
			/* 打印文件大小 */
			printf("%5ld",stat_0.st_size);
			
			printf(" ");
			
			/* 打印时间 */
			s_time = ctime(&stat_0.st_mtime);
			printf("%.12s",s_time + 4);
			
			printf(" ");
			
			/* 打印文件名 */
			printf("%s\r\n",dirent_0->d_name);
		}
	}
	
	
	return 0;
}









