#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define   	CMD_GET_KEY1  		_IOR('k',  1,  char*)
#define   	CMD_GET_KEY2  		_IOR('k',  2,  char*)

int main(int argc,char * argv[])
{
    int ret;
    int key1_fd,key2_fd;
    char buf[32] = {0};
    key1_fd = open(argv[1],O_RDWR);
    if(key1_fd < -1)
    {
        perror("---- open argv[1] error");
        exit(-1);
    }
    key2_fd = open(argv[2],O_RDWR);
    if(key2_fd < -1)
    {
        perror("---- open argv[2] error");
        exit(-1);
    }
    //printf("CMD_GET_KEY1 = %d CMD_GET_KEY2 = %d\n", CMD_GET_KEY1, CMD_GET_KEY2);
    while (1)
    {
        memset(buf, 0, sizeof(buf));
        ret = ioctl(key1_fd, CMD_GET_KEY1, buf);
        if(ret < 0)
        {
            perror("---- ioctl key1_fd error");
            exit(-1);
        }
        if(strcmp(buf, "key1") == 0)
        {
            printf("---- %s has been pressed!\n",buf);
        }


        memset(buf, 0, sizeof(buf));
        ret = ioctl(key2_fd, CMD_GET_KEY2, buf);
        if(ret < 0)
        {
            perror("---- ioctl key1_fd error");
            exit(-1);
        }
        if(strcmp(buf, "key2") == 0)
        {
            printf("---- %s has been pressed!\n",buf);
        }
    }

    close(key1_fd);
    close(key2_fd);
    return 0;
}





