#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

//定义一个端口 
#define  PORT  12400
//服务器的IP地址 
#define  IP "192.168.1.144"
#define  LISTENLENGTH 50	

int main()
{
	/*==================================== 函数返回值 ==========================================*/
	ssize_t  ret_sen;	//保存 send 函数的返回值
	int RetListen;	//保存 listen 函数的返回值
	int socketfd;	//保存 socket 的返回值,socket 文件描述符
	ssize_t  ret_rec;	//保存 recv 函数的返回值
	int ret_con;	//保存 recv 函数的返回值
	
	/*==================================== 函数参数 ==========================================*/
	socklen_t  AddrLength;	//bind 的第三个参数,bind 第二个参数类型的字节数
	struct  sockaddr_in addr;	// bind 的第二个参数
	
	/* ===================================================================================== */
	char buf[20] = {"0"};	
	char FileListFlag = 0;	//文进列表表已打印标志
	int file_count = 0;	//文件个数
	
	//创建socket套接字 
	socketfd = socket(AF_INET, SOCK_STREAM ,0 );
	if(socketfd < 0)
	{
		printf("scoket is error\r\n");
		return -1;
	}
	
	/* 地址结构提赋值,设置服务器IP和端口 */
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
	
	AddrLength = sizeof(addr);
	
	/* 请求链接到服务器 */
	ret_con = connect(socketfd,(struct sockaddr *)&addr,AddrLength);
	if(ret_con < 0)
	{
		printf("connect is error\r\n");
		return -1;
	}
	/*向服务器发送数据，请求获取文件列表数据*/
	ret_sen = send(socketfd,"a",1,0);
	if(ret_sen < 0)
	{
		printf("send is error\r\n");
		return -1;
	}
	/* 打印文件列表 */
	while(1)
	{
		ret_rec = recv(socketfd,buf,sizeof(buf),0);
		if(ret_rec == 0 && FileListFlag == 0)	//未收到到数据，未打印文件列表
		{
			printf("%d\r\n",ret_rec);
			continue;
		}
		else if(ret_rec < 0)	//接受数据失败
		{
			printf("%d\r\n",ret_rec);
			printf("recv error!\r\n");
			break;
		}
		else if(ret_rec > 0)	//接受成功，逐条打印收到的文件爱你条目
		{
			FileListFlag = 1;
			file_count++;
			printf("%s\r\n",buf);
		}
		else if(ret_rec == 0 && FileListFlag == 1)	//接受打印文件列表完毕
		{
			printf("%d\r\n",ret_rec);
			printf("显示共 %d 个文件\r\n",file_count);
			break;
		}
		printf("%d\r\n",ret_rec);
	}
	
	while(1)
	{
		
	}
	
	/* 关闭服务器端套接字文件 */
	close(socketfd);
	
	return 0;
}


