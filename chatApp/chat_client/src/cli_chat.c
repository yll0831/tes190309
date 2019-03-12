#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include "Double_Link.h"
#include "cli_chat.h"
#include "unistd.h"


#define  PORT  10087            //服务器的端口号 
#define  IP "192.168.10.221"    //服务器的IP地址  
#define  SENDBUFSIZE 60         //最大发送缓冲区大小
#define  RECBUFSIZE 60          //最大接收缓冲区大小
#define  CLIFDBUFSIZE 10        //套套接字fd的缓冲区大小
#define  NEWCLIHEAD 6      //新用户标识符缓冲区大小
#define  JOINHEAD 4     //注册判断
#define  LOGINHEAD 5    //登入判断
#define  CURCLIHEAD 6   //用户信息判断
#define  ALLCLIHEAD 6
#define ONLINE 1
#define OFFLINE 0
#define  STATEHEAD 5

char recBuf[RECBUFSIZE] = {0};  //发送缓冲区
char sendBuf[SENDBUFSIZE] = {0};    

NODE curCliNode;    //当前用户节点,临时结点



void * tha_fun(void * arg)
{
    char cliFdBuf[CLIFDBUFSIZE + 1] = {0};  //用户标识符缓冲区
    int ret;
    int cliSocketFd,i = 0; //
    NODE * nodePtr;     //节点指针
    int dLinkSize;      //暂存双向链表表节点个数
    NODE cliNode;
    while(1)
    {
        memset(recBuf,0,RECBUFSIZE);
        ret = recv((int)arg,recBuf,RECBUFSIZE,MSG_DONTWAIT);    //非阻塞接收数据
        if(ret > 0)
        {   
            //printf("000000000000%s %s\n",recBuf,recBuf + STATEHEAD + CLIFDBUFSIZE + 1);
            if((strncmp(recBuf,"state",STATEHEAD) == 0))  //有用户登入或下线
            {
                //获取用户socketFd
                memset(cliFdBuf,0,sizeof(cliFdBuf));
                strncpy(cliFdBuf,recBuf + STATEHEAD,CLIFDBUFSIZE);
                cliSocketFd = atoi(cliFdBuf);
                nodePtr = Search_ByName(recBuf + STATEHEAD + CLIFDBUFSIZE + 1);
                if(nodePtr == NULL)
                {
                    printf("%s:no such user!\n",__func__);
                    return;
                }
                nodePtr->cliSocketFd = cliSocketFd;
                nodePtr->cliState = *(recBuf + STATEHEAD + CLIFDBUFSIZE) - 48;

                Display_Friend_List();  
                //printf("--CLIENT--:%s:New users are coming online,Please select a chat object:\n",curCliNode.userName);
            }
            else if(strncmp(recBuf,"newcli",NEWCLIHEAD) == 0)
            {
                //获取用户名
                cliNode.cliState = *(recBuf + NEWCLIHEAD) - 48;
                strcpy(cliNode.userName,recBuf + NEWCLIHEAD + 1);

                /* 新建节点，成功：返回节点指针，失败：返回NULL */
                nodePtr =  Create_Node(&cliNode);
                if(nodePtr == NULL)
                {
                    printf("--CLIENT--:%s:Create_Node error!\n",__func__);
                    return ;
                }

                /* 将新节点插入链表末尾 */
                Dlink_Insert_Last(nodePtr);
                Display_Friend_List();
            }
            else    //接收消息
            {
                //获取发送方套接字fd
                memset(cliFdBuf,0,CLIFDBUFSIZE + 1);
                strncpy(cliFdBuf,recBuf,CLIFDBUFSIZE);
                cliSocketFd = atoi(cliFdBuf);

                //打印接收到的消息
                nodePtr = Get_First_Node();
                dLinkSize = Dling_Size();
                i = 0;
                while(i < dLinkSize)
                {
                    if(nodePtr->cliSocketFd == cliSocketFd)
                    {
                        printf("--CLIENT--:receive from %s:\n%s\n",nodePtr->userName,recBuf + CLIFDBUFSIZE);
                        break;
                    }
                    nodePtr = nodePtr->next;
                    i++;
                }
                if(i == dLinkSize)
                {
                    printf("--CLIENT--:There is no such cliSocketFd %d in the DLink!\n",cliSocketFd);
                }
            }
            
        }
    }
}

int main()
{
	 ssize_t  ret;
	// socklen_t  svrAddrLength;  // struct socksvrAddr_in 类型的长度
	// struct  sockaddr_in svrAddr;
    int svrSocketFd;    //服务端socketfd
    int multiUse;   //计数
    NODE * nodePtr;
    int dLinkSize;      //暂存双向链表表节点个数

    pthread_t thaId;    
	
    
    //初始化本客户端
    svrSocketFd = Init_Client();
    if(svrSocketFd < 0)
    {
        printf("--CLIENT--:Init_Client error!\n");
        return -1;
    }
    

    
    //创建线程
    ret = pthread_create(&thaId,NULL,tha_fun,(void *)svrSocketFd);
    if(ret < 0)
    {
        printf("--CLIENT--:pthread_create error!\n");
        return -1;
    }
    ret = pthread_detach(thaId);
    if(ret != 0)
    {
        printf("--CLIENT--:pthread_detach error!\n");
    }

    //Display_Friend_List();  
    while(1)
    {
        dLinkSize = Dling_Size();
        if(dLinkSize)
        {
            memset(sendBuf,0,SENDBUFSIZE);
            while(1)
            {
                //获取目标用户
                printf("--CLIENT--:%s:Please select a chat object:\n",curCliNode.userName);
                scanf("%s",sendBuf);
                if(strcmp(sendBuf, "exit") == 0)
                {
                    Send_OffLineSignal(svrSocketFd);
                    return -1;
                }
                multiUse = 0;
                nodePtr = Get_First_Node();
                while(multiUse < dLinkSize)
                {
                    if(strcmp(sendBuf,nodePtr->userName) == 0)
                    {
                        memset(sendBuf,0,SENDBUFSIZE);
                        sprintf(sendBuf,"%d",nodePtr->cliSocketFd); //写入目标用户fd
                        break;
                    }
                    nodePtr = nodePtr->next;
                    multiUse++;
                }
                if(multiUse != dLinkSize)
                {
                    break;
                }
                else if(multiUse == dLinkSize) 
                {
                    printf("--CLIENT--:The user does not exist!\n");
                }
                
            }

            //获取发送内容
            printf("--CLIENT--:%s:Please enter the message you want to send:\n",curCliNode.userName);
            scanf("%s",sendBuf + CLIFDBUFSIZE);
            if(strcmp(sendBuf + CLIFDBUFSIZE, "exit") == 0)
            {
                Send_OffLineSignal(svrSocketFd);
                return -1;
            }
            //发送消息到服务器
            ret = send(svrSocketFd,sendBuf,CLIFDBUFSIZE + strlen(sendBuf + CLIFDBUFSIZE) + 1,0);
            if(ret < 0)
            {
                printf("--CLIENT--:%s：Fail to send\n",curCliNode.userName);
                return -1;
            }
            
            printf("--CLIENT--:%s:send successfully\n",curCliNode.userName);
        }
    }
	
	//close(svrFd);
	
	return 0;
}

int Conect_Server(void)
{
    ssize_t  ret;
	socklen_t  svrAddrLength;  // struct socksvrAddr_in 类型的长度
	struct  sockaddr_in svrAddr;
    int svrSocketFd;    //服务端socketfd

    //创建socket套接字 
	svrSocketFd = socket(AF_INET, SOCK_STREAM ,0 );
	if(svrSocketFd < 0)
	{
		printf("--CLIENT--:%s:scoket is error\n",__func__);
		return -1;
	}

	svrAddrLength = sizeof(svrAddr);
	memset(&svrAddr,0,sizeof(svrAddr));
	//填入服务器IP和端口
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(PORT);
	svrAddr.sin_addr.s_addr = inet_addr(IP);
	
	//请求连接到服务器
	ret = connect(svrSocketFd,(struct sockaddr *)&svrAddr,svrAddrLength);
	if(ret < 0)
	{
		printf("--CLIENT--:%s:connect is error\n",__func__);
		return -1;
	}

    return svrSocketFd;
}

int Join(void)
{
    char uNameBuf[USERNAMESIZE + JOINHEAD + 1] = {0};
    int svrSocketFd,cliSocFd;
    ssize_t ret;
    svrSocketFd = Conect_Server();
    if(svrSocketFd < 0)
    {
        printf("%s:Conect_Server error!\n",__func__);
        return -1;
    }
    //获取用户名
    printf("--CLIENT--:Please enter your new user name(Not more than 15 characters):\n");
    while(1)
    {   
        memset(uNameBuf,0,USERNAMESIZE + JOINHEAD + 1);
        sprintf(uNameBuf,"Join");   //填入注册标志
        //read(0,uNameBuf + JOINHEAD,USERNAMESIZE);       //填入用户名
        scanf("%s",uNameBuf + JOINHEAD);//填入用户名
        ret = send(svrSocketFd,uNameBuf,strlen(uNameBuf) + 1,0);    //发送注册请求
        if(ret < 0)
        {
            printf("--CLIENT--:%s:send join request error!\n",__func__);
            return -1;
        }

        memset(uNameBuf,0,USERNAMESIZE + JOINHEAD + 1); //清空uNameBuf 作为接收buf
        printf("Wait for a moment please\n");
        ret = recv(svrSocketFd,uNameBuf,USERNAMESIZE + JOINHEAD + 1,0); //接收注册请求应答
        if(ret < 0)
        {
            printf("--CLIENT--:%s:recv error!\n",__func__);
            return -1;
        }
        if(strncmp(uNameBuf,"Joinok",JOINHEAD + 2) == 0)
        {
            printf("--CLIENT--:joined successfully!\n");
            break;
        }
        else
        {
            printf("--CLIENT--:The user name already exists,please enter again:\n");
        }
    }
    //cliSocFd = atoi(uNameBuf + JOINHEAD + 2);
    //close(cliSocFd);
    //close(svrSocketFd);
    return 0;
}


int Login(void)
{
    int svrSocketFd;
    char uNameBuf[USERNAMESIZE + LOGINHEAD + 1] = {0};
    ssize_t ret;
    svrSocketFd = Conect_Server();
    if(svrSocketFd < 0)
    {
        printf("%s:Conect_Server error!\n");
        return -1;
    }
    //获取用户名
    printf("--CLIENT--:Please enter your user name(Not more than 15 characters):\n");
    while(1)
    {   
        memset(uNameBuf,0,USERNAMESIZE + LOGINHEAD + 1);    
        sprintf(uNameBuf,"Login");  //填入登入标志
       //read(0,uNameBuf + LOGINHEAD,USERNAMESIZE);  //填入用户名
        scanf("%s",uNameBuf + LOGINHEAD);//填入用户名
        if(strcmp(uNameBuf + LOGINHEAD,"exit") == 0)
        {
            return -1;
        }
        strcpy(curCliNode.userName,uNameBuf + LOGINHEAD);   //保存本客户端用户名

        ret = send(svrSocketFd,uNameBuf,strlen(uNameBuf) + 1,0);    //发送登入请求
        if(ret < 0)
        {
            printf("--CLIENT--:%s:send error!\n",__func__);
            return -1;
        }
        memset(uNameBuf,0,USERNAMESIZE + LOGINHEAD + 1);
        printf("Wait for a moment please\n");

        ret = recv(svrSocketFd,uNameBuf,USERNAMESIZE + LOGINHEAD + 1,0);    ////接收登入请求应答
        if(ret > 0)
        {
            if(strncmp(uNameBuf,"Loginok",LOGINHEAD + 2) == 0)
            {
                curCliNode.cliSocketFd = atoi(uNameBuf + LOGINHEAD + 2);    //保存本客户端的fd
                printf("--CLIENT--:Login successful!:%d\n",curCliNode.cliSocketFd);         //////
                break;
            }
            else
            {
                printf("--CLIENT--:Account does not exist or already logined,please enter again:\n");
            }
        }
    }
    return svrSocketFd;
}

//初始化客户端
int Init_Client(void)
{
    int ret;
    NODE * nodePtr,cliNode;
    char buf[50] = {0};
    char button;
    int svrSocketFd;
    
    //gethostname(buf,sizeof(buf));
    //fprintf(stdout,"--CLIENT--:HOST_NAME:%s\n",buf);   //打印本客户端主机名

    //* 新建双向链表，成功：返回 0，失败：返回 -1 */
    ret = Create_Dlink();
    if(ret != 0)
    {
        printf("--CLIENT--:create double link error!\n");
        return -1;
    }

    while(1)
    {
        printf("please select:\n'l' to Login\n'j' to Join\n");
        //memset(buf,0,sizeof(buf));
        button = 0;
        read(0,&button,1);
        fgets(buf,sizeof(buf),stdin);   //清空标准输入缓冲区
        if(button == 'l')
        {
            svrSocketFd = Login();
            if(svrSocketFd > 0)
            {
                break;
            }
            else
            {
                return -1;
            }
            
        }
        else if(button == 'j')
        {
            ret = Join();
            if(ret < 0)
            {
                return -1;
            }
        }
        else
        {
            printf("Incorrect input\n");
        }
    }
    
    
   
    //接收当前在线用户的信息
    while(1)
    {
        memset(recBuf,0,RECBUFSIZE);
        ret = recv(svrSocketFd,recBuf,RECBUFSIZE,0);   //MSG_DONTWAIT 
        if(ret > 0)
        { 
            //printf("%s %s\n",recBuf,recBuf + ALLCLIHEAD + CLIFDBUFSIZE);    ////////////////////////////
            if(strncmp(recBuf,"allcli",ALLCLIHEAD) == 0)
            {
                memset(buf,0,sizeof(buf));
                strncpy(buf,recBuf + ALLCLIHEAD,CLIFDBUFSIZE); //获取用户fd
                cliNode.cliSocketFd = atoi(buf);
                cliNode.cliState = *(recBuf + ALLCLIHEAD + CLIFDBUFSIZE) - 48;
                //printf("0--------------%u\n",cliNode.cliState); //////////////////
                strcpy(cliNode.userName,recBuf + ALLCLIHEAD + CLIFDBUFSIZE + 1);   //获取用户名
                //printf("fd = %d,name = %s,state = %u\n",cliNode.cliSocketFd,cliNode.userName,cliNode.cliState);      //////////////////////////////
                //printf("%d %s %u\n",cliNode.cliSocketFd,cliNode.userName,cliNode.cliState);       ////////////////////////////////////////
                /* 新建节点，成功：返回节点指针，失败：返回NULL */
                nodePtr =  Create_Node(&cliNode);
                if(nodePtr == NULL)
                {
                    printf("--CLIENT--:Create_Node error!\n");
                    return -1;
                }
                //printf("fd = %d,name = %s,state = %u\n",nodePtr->cliSocketFd,nodePtr->userName,nodePtr->cliState);      //////////////////////////////
                /* 将新节点插入链表末尾 */
                Dlink_Insert_Last(nodePtr);
            }
            else if(strncmp(recBuf,"end",3) == 0) //获取所有用户信息完毕
            {
                break;
            }
        }
    }

    //Display_Friend_List();

    return svrSocketFd;
}

void Display_Friend_List(void)
{
    int i,count,dLinkSize;
    NODE * nodePtr;
    dLinkSize = Dling_Size();
    i = 0;
    count = 0;
    if(dLinkSize)
    {   
        nodePtr = Get_First_Node();
        printf("========== friend list start =========\n");
        
        printf("|ONLINE PEOPLE|:\n");
        while(i++ < dLinkSize)
        {
            if(nodePtr->cliState == ONLINE)
            {
                //printf("%d.cliSocketFd:%5d | userName:%s\n",i,nodePtr->cliSocketFd,nodePtr->userName);
                printf("%d.%s \n",count + 1,nodePtr->userName);
                count++;
            }
            nodePtr = nodePtr->next; 
        }
        printf("total:%2d\n\n",count);
        nodePtr = Get_First_Node();
        i = 0;
        count = 0;
        printf("|OFFLINE PEOPLE|:\n");
        while(i++ < dLinkSize)
        {
            if(nodePtr->cliState == OFFLINE)
            {
                //printf("%d.cliSocketFd:%5d | userName:%s\n",i,nodePtr->cliSocketFd,nodePtr->userName);
                printf("%d.%s \n",count + 1,nodePtr->userName);
                count++;
            }
            nodePtr = nodePtr->next; 
        }
        printf("total:%2d\n",count);
        printf("======================================\n");
    }
    // else 
    // {
    //     printf("NUMBER OF ONLINE PEOPLE:%2d\n",1);
    // }
}


void Send_OffLineSignal(int svrSocketFd)
{
    int ret;
    ret = send(svrSocketFd,"exit",5,0);
    if(ret < 0)
    {
        printf("--CLIENT--:%s:send error!\n",__func__);
        //return -1;
    }
}

NODE * Search_BySocketFd(int cliSocketFd)
{
    int count,dLinkSize;
    NODE * nodePtr;
    dLinkSize = Dling_Size();
    if(dLinkSize)
    {
        nodePtr = Get_First_Node();
    }
    count = 0;
    while(count < dLinkSize)
    {
        if(nodePtr->cliSocketFd == cliSocketFd)
        {
            return nodePtr;
        }
        count++;
        nodePtr = nodePtr->next;
    }
    return NULL;
}

//根据用户名判断指定节点是否在链表中
NODE * Search_ByName(char * uName)
{
    int count,dLinkSize;
    NODE * nodePtr;
    dLinkSize = Dling_Size();
    if(dLinkSize)
    {
        nodePtr = Get_First_Node();
    }
    count = 0;
    while(count < dLinkSize)
    {
        if(strcmp(nodePtr->userName,uName) == 0)
        {
            return nodePtr;
        }
        count++;
        nodePtr = nodePtr->next;
    }
    return NULL;
}