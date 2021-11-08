#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{	
	int ret_fd;
	ret_fd = open("2.data",O_RDWR);
	if(ret_fd < 0)
	{
		perror("open:");
		return -1;
	}
	
	ftruncate(ret_fd,1024);
	
	close(ret_fd);
	
	return 0;
}
