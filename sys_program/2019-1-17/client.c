#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

//定义一个端口 
#define  PORT  10086 
//服务器的IP地址 
#define  IP "192.168.1.111"
#define  LISTENLENGTH 50	

int main()
{
	ssize_t  ret;
	int NewId;
	int RetListen;
	int  retbid;
	int socketfd;
	socklen_t  AddrLength;
	struct  sockaddr_in addr;
	char buf[200] = {"？？"};
	
	//创建socket套接字 
	socketfd = socket(AF_INET, SOCK_STREAM ,0 );
	if(socketfd < 0)
	{
		printf("scoket is error\r\n");
		return -1;
	}
	//设置服务器IP和端口
	//长度
	AddrLength = sizeof(addr);
	//在填充成员之前  清空结构体 
	memset(&addr,0,sizeof(addr));
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	
	//请求链接
	ret = connect(socketfd,(struct sockaddr *)&addr,AddrLength);
	if(ret < 0)
	{
		printf("connect is error\r\n");
		return -1;
	}
	//发送消息
	ret = send(socketfd,buf,sizeof(buf),0);
	if(ret < 0)
	{
		printf("send is error\r\n");
		return -1;
	}
	
	printf("send is ok\r\n");
	
	close(socketfd);
	
	return 0;
}


