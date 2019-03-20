#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFSIZE 32

int main(int argc, char* argv[])
{
	int led_fd,key_fd;
	led_fd = open(argv[1], O_RDWR);
    char buf[BUFSIZE] = {0};
    unsigned int value;
	if(led_fd < 0)
	{
		printf("open %s failed!\n", argv[1]);
		return 0;
	}
    key_fd = open(argv[2], O_RDWR);
	if(key_fd < 0)
	{
		printf("open %s failed!\n", argv[2]);
		return 0;
	}
	
    
	while(1)
	{
		memset(buf, 0, BUFSIZE);
		read(key_fd,buf,BUFSIZE);
        value = atoi(buf);
        value = value & ( 1 << (1 * 2) );		//key1
        if(!value)
        {
		    write(led_fd, "led_on", 6);
        }
        read(key_fd,buf,BUFSIZE);
        value = atoi(buf);
        value = value & ( 1 << (1 * 3) );		//key2
        if(!value)
        {
            write(led_fd, "led_off", 7);
        }
	}



	close(led_fd);
    close(key_fd);
	return 0;
}
