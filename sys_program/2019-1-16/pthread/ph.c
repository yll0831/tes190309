#include <stdio.h>
#include <pthread.h>


void *fun (void *arg) 
{
	
	printf("i am   child   pthread\r\n");
	
	sleep(1);
	printf("recv:%d\r\n",arg);
	
}
void *fun2 (void *arg) 
{
	printf("i am   child2   pthread\r\n");
	//pthread_exit(NULL);
	sleep(2);
	printf("recv:%d\r\n",(int)arg);	
}
int main(int argc,char *argv[])
{
	int a = 8;
	int b = 1;
	 pthread_t  th;
	  pthread_t th2;

	 int ret = pthread_create(&th,NULL,fun,(void *)a);
	 if(ret < 0)
	 {
		printf("pthread create is error\r\n");
		return -1;
	 }
	  int ret1 = pthread_create(&th2,NULL,fun2,(void *)b);
	 if(ret1 < 0)
	 {
		printf("pthread create is error\r\n");
		return -1;
	 }
		
	 printf(" i am parent...........\r\n");
	 
	 
	 pthread_exit(NULL);
	// pthread_join(th,NULL);
	// pthread_join(th2,NULL);
	 
 printf(" i am parent\r\n");
	 
	return 0;
}
