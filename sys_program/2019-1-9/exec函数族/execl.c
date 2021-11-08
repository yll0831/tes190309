#include <stdio.h>
#include <unistd.h>

/* l表示把传递给程序的参数依次列出来*/
int main()
{
	//execl("/bin/pwd","pwd",NULL);
	execl("/bin/ls","ls","-l",NULL);
	//execl("hei","hei",NULL);
	return 0;
}
