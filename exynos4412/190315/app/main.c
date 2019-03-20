#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

int fd;

//线程函数

/*void* led_ctl(void* arg)
{
	char a = (char)(arg+48);
	printf("arg=%c\n",a);

	write(fd,&a,1);
	return (void*)0;
}*/


int main(int argc,char* argv[])
{
	pthread_t pthnum;
	char buf[12];
	int flag = 0,loosen = 0;
	//pthnum = pthread_self();			//线程号获取
	//int convey;							//传递的数据
	fd = open(argv[1],O_RDWR); 
	if(fd < 0)
	{
		perror("error\n");
		return -1;
	}
	while(1)
	{
		memset(buf,0,sizeof(buf));
		loosen = 0;
		read(fd,buf,sizeof(buf));
		if(strncmp(buf,"key",3)==0)
		{	loosen = 1;
			if((strncmp(buf,"key",3)==0) && (flag == 0))
			{	flag = 1;
				printf("%s is pressed!\n",buf);
				//convey = (int)buf[3];//convey = ((int*)buf[3])-48 !!!
				//convey -= 48;
				write(fd,&buf[3],1);
				//pthread_create(&pthnum,NULL,led_ctl, (void*)convey);
			}
		}
		usleep(10000);
		if(loosen == 0)
		{
			flag = 0;
		}
		
	}	
		close(fd);
		return 0;
	}
