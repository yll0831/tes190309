#if 1
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

/*=================================================
生产者 生产  5个  消费消费5个  生产继续生产  消费者继续消费 
==================================================*/


sem_t sem;	//定义信号量，商品的个数
int product_count = 0;	//商品个数

/* 生产者线程函数 */
void * producer(void * arg)
{
	while(1)
	{
		if(product_count == 0)
		{
			/* p 操作 */
			sem_wait(&sem);
			while(1)
			{
				sleep(1);
				printf("producer:当前商品数量：%d\r\n",++product_count);
				if(product_count == 5)
				{
					/* v 操作 */
					sem_post(&sem);
					break;
				}
			}
		}
		
	}
	
}

/* 消费者线程函数 */
void * consumer(void * arg)
{
	while(1)
	{
		if(product_count == 5)
		{
			/* p 操作 */
			sem_wait(&sem);
			while(1)
			{
				sleep(1);
				printf("consumer:当前商品数量：%d\r\n",--product_count);
				if(product_count == 0)
				{
					/* v 操作 */
					sem_post(&sem);
					break;
				}
			}
			
		}
	}
	
}


int main()
{
	int ret_pc;	//保存 pthread_create 的返回值
	
	pthread_t th_producer;	//保存生产者线程 id， pthread_create 的第一个参数
	pthread_t th_consumer;	//保存消费者线程 id
	
	/* 初始化信号量 */
	sem_init(&sem,0,1);
	
	/* 创建生产者线程 */
	ret_pc = pthread_create(&th_producer,NULL,producer,NULL);
	if(ret_pc < 0)
	{
		printf("pthread_create th_producer error!\r\n");
		return -1;
	}
	
	/* 创建消费者线程 */
	ret_pc = pthread_create(&th_consumer,NULL,consumer,NULL);
	if(ret_pc < 0)
	{
		printf("pthread_create th_producer error!\r\n");
		return -1;
	}
	
	/* 等待生产者线程结束 */
	pthread_join(th_producer,NULL);
	
	/* 等待消费者线程结束 */
	pthread_join(th_consumer,NULL);
	
	return 0;
}
#endif

#if 0
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;
sem_t sem1;
//定义锁 
pthread_mutex_t  mutex;

void *fun (void *arg)   //窗口一 
{
	while(1)
	{
		//p操作
		sem_wait(&sem);  // 1- 1 = 0 
		
		printf("i am child 1\r\n");
		sleep(1);
	   // v操作
	   sem_post(&sem1); // 0+1 = 1
	}
}

void *fun2 (void *arg)   //窗口 二  
{ 
	while(1)
	{
		// p 操作
		sem_wait(&sem1);  // 1 - 1 = 0
		printf("i am child 2\r\n");
		sleep(1);
		//v操作
		sem_post(&sem);  // 0 + 1 = 1
	}
}
int main(int argc,char *argv[])
{
	
	 pthread_t  th;
	 pthread_t th2;
	
	//初始化一个信号量
	sem_init(&sem,0,0);
	sem_init(&sem1,0,1);
	
	 int ret = pthread_create(&th,NULL,fun,NULL);
	 if(ret < 0)
	 {
		printf("pthread create is error\r\n");
		return -1;
	 }
	 int ret1 = pthread_create(&th2,NULL,fun2,NULL);
	 if(ret1 < 0)
	 {
		printf("pthread create is error\r\n");
		return -1;
	 }
	 printf(" i am parent...........\r\n");
	 
	
	 pthread_join(th,NULL);
	 pthread_join(th2,NULL);
	 printf(" i am parent\r\n");
	 
	return 0;
}
#endif






