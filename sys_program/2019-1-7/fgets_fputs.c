#include <stdio.h>
#include <string.h>

#define N  100

int main()
{
	char buf[N] = {'0'};
	
	//源文件
	FILE *p  = fopen("hello.data","r+");
	if(NULL == p)
	{
		printf("fopen is error\r\n");
		return  -1;
	}
	char *  c;
	int n;
	int i = 0;
	while(1)
	{
		c = fgets(buf,N-1,p);
		if(c == NULL)
		{
			break;
		}
        printf("%s",buf);
		n = strlen(buf);
		if(buf[n-1] == '\n' )
		{
			
			i++;
		}
		
	}
	printf("i = %d\r\n",i);
	
	fclose(p);
	return 0;
}
//结果
/*
hello
fdfsd
fsdfsdfs
rtrw
gxf
fhsd
i = 6
*/