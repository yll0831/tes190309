#include <stdio.h>
#include <pthread.h>

/*=================================================
模拟 火车票的售票系统   有两个售票窗口   窗口一  卖一张票  窗口二  卖一张 票 直到卖完为止 。总共 10张票
==================================================*/

int train_ticket = 10;	//火车票余量

//定义锁 
pthread_mutex_t  mutex;

/* 窗口 1 */
void * windows_1(void * arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);	//加锁
		if(train_ticket > 0)
		{
			printf("窗口1卖出一张,剩余 %d 张\r\n",--train_ticket);
		}
		else
		{
			printf("窗口1已无票\r\n");
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);
		}
		pthread_mutex_unlock(&mutex);	//解锁
		sleep(1);
	}
	
}
/* 窗口 2 */
void  * windows_2(void * arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);	//加锁
		if(train_ticket > 0)
		{
			printf("窗口2卖出一张,剩余 %d 张\r\n",--train_ticket);
		}
		else
		{
			printf("窗口2已无票\r\n");
			pthread_mutex_unlock(&mutex);
			pthread_exit(0);	
		}
		pthread_mutex_unlock(&mutex);	//解锁
		sleep(1);
	}
	
}

int main()
{
	int ret_pt;		//保存 pthread_create 函数的返回值
	
	pthread_t th_1,th_2;	//保存线程id
	
	/* 初始化互斥锁 */
	pthread_mutex_init(&mutex,NULL);
	
	/* 创建线程 */
	ret_pt = pthread_create(&th_1,NULL,windows_1,NULL);
	if(ret_pt < 0)
	{
		printf("pthread th_1 create error\r\n");
		return -1;	
	}
	
	/* 创建线程 */
	ret_pt = pthread_create(&th_2,NULL,windows_2,NULL);
	if(ret_pt < 0)
	{
		printf("pthread th_2 error\r\n");
		return -1;	
	}
	
	
	/* 回收线程 */
	pthread_join(th_1,NULL);
	pthread_join(th_2,NULL);
	
	return 0;
	
}
