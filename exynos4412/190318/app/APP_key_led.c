#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
//#include <linux/delay.h>
#include <pthread.h>
#include <unistd.h>
#define BUFSIZE 32

unsigned char on_flag = 1;

void * tha_fun(void * arg)
{
    int a;
    while(1)
    {
        while(on_flag)
        {
            write((int)arg, "led1_on", 7);
            write((int)arg, "led2_off", 8);
            //write((int)arg, "led3_off", 8);
            usleep(1000 * 300);

            write((int)arg, "led1_off", 8);
            write((int)arg, "led2_on", 7);
            //write((int)arg, "led3_off", 8);
            usleep(1000 * 300);

            // write((int)arg, "led1_off", 8);
            // write((int)arg, "led2_off", 8);
            // write((int)arg, "led3_on", 7);
            // usleep(1000 * 300);
        }
    }
}

int main(int argc, char* argv[])
{
	int key_led_fd,ret;
    char buf[BUFSIZE] = {0};
	key_led_fd = open(argv[1], O_RDWR);
    pthread_t thaId;
	if(key_led_fd < 0)
	{
		printf("open %s failed!\n", argv[1]);
		return 0;
	}

    
    ret = pthread_create(&thaId,NULL,tha_fun,(void *)key_led_fd);
    if(ret < 0)
    {
        printf("----:pthread_create error!\n");
        return -1;
    }
    ret = pthread_detach(thaId);
    if(ret != 0)
    {
        printf("----:pthread_detach error!\n");
    }
	while(1)
	{
		memset(buf, 0, BUFSIZE);
		read(key_led_fd,buf,BUFSIZE);
        if(strncmp(buf,"key1_on",7) == 0)
        {
		    on_flag = 1;
        }
        else if(strncmp(buf,"key2_on",7) == 0)
        {
		    on_flag = 0;
        }
	}

	close(key_led_fd);
	return 0;
}
