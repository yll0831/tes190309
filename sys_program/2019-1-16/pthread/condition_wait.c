#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>



pthread_cond_t  cond;	//定义条件变量


pthread_mutex_t  mutex;	//定义锁变量


/* 线程处理函数 */
void *fun (void *arg)   
{
	printf("正在加载......\r\n");
	//加锁成功  0值 
	printf("当前线程已加锁成功:%d\r\n",pthread_mutex_lock(&mutex));
	//等待的条件
	pthread_cond_wait(&cond,&mutex);
	printf("激活成功......\r\n");
	
}

int main(int argc,char *argv[])
{
	int ret_pc;		//存储 pthread_create 的返回子值
	pthread_t  th;	//存储 线程id pthread_create的 第一个参数
	
	/* 初始化一个条件变量 */
	pthread_cond_init(&cond,NULL);
	
	/* 初始化一把锁 */
	pthread_mutex_init(&mutex,NULL);
	
	/* 创建线程 */
	ret_pc = pthread_create(&th,NULL,fun,NULL);
	if(ret_pc < 0)
	{
		printf("pthread create is error\r\n");
		return -1;
	}
	
	printf(" i am parent...........\r\n");
	sleep(5);
	 
	/* 激活条件变量 */
	pthread_cond_signal(&cond);
	 
	/* 等待线程结束 */
	pthread_join(th,NULL);
	 
	return 0;
}

