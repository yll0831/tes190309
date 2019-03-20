#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd = 0;
	fd = open(argv[1], O_RDWR);
	if(fd < 0)
	{
		printf("open %s failed!\n", argv[1]);
		return 0;
	}

	char  buf[32] = {0};
	while(1)
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), stdin);
		write(fd, buf, strlen(buf));
	}

	close(fd);
	return 0;
}
