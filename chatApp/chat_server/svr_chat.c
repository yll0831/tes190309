#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "Double_Link.h"
#include <unistd.h>
#include "svr_chat.h"

#define  PORT  10087           //服务器端口
#define  IP "192.168.10.221"    //服务器ip
#define  LISTENLENGTH 50        //监听客户端数量
#define  SENDBUFSIZE 60         //最大发送缓冲区大小
#define  RECBUFSIZE 60          //最大接收缓冲区大小
#define  CLIFDBUFSIZE 10        //套套接字fd的缓冲区大小
#define  NEWCLIHEAD 6      //新用户标识符缓冲区大小
#define  JOINHEAD 4     //注册判断
#define  LOGINHEAD 5    //登入判断
#define  CURCLIHEAD 6   //用户信息判断
#define  ALLCLIHEAD 6
#define  ONLINE 1
#define  OFFLINE 0
#define  STATEHEAD 5


void * tha_fun(void * arg)
{
    NODE * nodePtr;
    int i,ret = 0;
    char cliFdBuf[CLIFDBUFSIZE + 1] = {0};  //客户端fd缓冲区
    int cliSocketFd;    
    char recBuf[RECBUFSIZE];
    int dLinkSize;  //暂存双向链表的节点个数
    
    while(1)
    {
        dLinkSize = Dling_Size();   
        if(dLinkSize)   //有用户才获取第一个节点
        {
            nodePtr = Get_First_Node();
        }
        for(i = 0;i < dLinkSize;i++)
        {
            if(nodePtr->cliState == ONLINE)
            {
                memset(recBuf,0,RECBUFSIZE);
                ret = recv(nodePtr->cliSocketFd,recBuf,RECBUFSIZE,MSG_DONTWAIT);    //非阻塞接收数据
                if(ret > 0)
                {
                    if(strcmp(recBuf,"exit") == 0)
                    {
                        nodePtr->cliState = OFFLINE;
                        Send_ClientState(nodePtr);
                        close(nodePtr->cliSocketFd);
                        Display_ALL_User_Node();
                    }
                    else
                    {
                        printf("--SERVER--:receive from %s:%s\n",nodePtr->userName,recBuf + CLIFDBUFSIZE);
                        //获取目标用户fd
                        memset(cliFdBuf,0,CLIFDBUFSIZE + 1);
                        strncpy(cliFdBuf,recBuf,CLIFDBUFSIZE);
                        cliSocketFd = atoi(cliFdBuf);  

                        //填入数据源端fd
                        memset(cliFdBuf,0,CLIFDBUFSIZE + 1);
                        sprintf(cliFdBuf,"%d",nodePtr->cliSocketFd);
                        strncpy(recBuf,cliFdBuf,CLIFDBUFSIZE);
                        ret = send(cliSocketFd,recBuf, CLIFDBUFSIZE + strlen(recBuf + CLIFDBUFSIZE) + 1,0);
                        if(ret < 0)
                        {
                            printf("--SERVER--:%s send is error\n",__func__);
                        }
                        printf("--SERVER--:send to %d success\n",cliSocketFd);
                    }
                }
            }
            nodePtr = nodePtr->next;
        }
    }
}


int main()
{
    int svrSocketFd;  //服务端socketfd
    int cliSocketFd;  //客户端socketfd
	int  ret,i;      
	socklen_t  addrLength;  
	struct  sockaddr_in svrAddr;    //保存服务端地址信息
    //struct  sockaddr_in cliAddr;    //保存客户端地址信息
    char * cliAddr_socFd;
    char hostName[30] = {0};    //保存主机名字
    pthread_t thaId;
    //NODE newNode;
    //int dLinkSize;  //暂存双向链表的节点个数

    gethostname(hostName,sizeof(hostName)); //获取主机名字
    fprintf(stdout,"--SERVER--:HOST_NAME:%s\n",hostName);   //打印服务器端主机名
	
	//创建socket套接字 
	svrSocketFd = socket( AF_INET, SOCK_STREAM ,0);
	if(svrSocketFd < 0)
	{
		printf("--SERVER--:scoket is error\r\n");
		return -1;
	}
    
	memset(&svrAddr,0,sizeof(svrAddr));
	addrLength = sizeof(svrAddr);
	
	svrAddr.sin_family = AF_INET;
	svrAddr.sin_port = htons(PORT);
	svrAddr.sin_addr.s_addr = inet_addr(IP);
	
	//绑定一个端口与IP
	ret = bind(svrSocketFd,(struct sockaddr *)&svrAddr,addrLength);
	if(ret < 0)
	{
		printf("--SERVER--:bind id error\r\n");
		return -1;
	}
    //创建线程
    ret = pthread_create(&thaId,NULL,tha_fun,NULL);
    if(ret < 0)
    {
        printf("--SERVER--:pthread_create error!\n");
        return -1;
    }
    ret = pthread_detach(thaId);
    if(ret != 0)
    {
        printf("--SERVER--:pthread_detach error!\n");
    }

    //监听客户端
    ret = listen(svrSocketFd,LISTENLENGTH);
    if(ret < 0)
    {
        printf("--SERVER--:listen is error\r\n");
        return -1;
    }
    
    //* 新建双向链表，成功：返回 0，失败：返回 -1 */
    ret = Create_Dlink();
    if(ret != 0)
    {
        printf("--SERVER--:create double link error!\n");
        return -1;
    }
    
    
    while(1)
    {
        printf("listening for client request...\r\n");

        cliAddr_socFd = malloc(sizeof(int) + sizeof(struct  sockaddr_in));  //分配动态空间，存储fd和地址信息
        memset(cliAddr_socFd,0,sizeof(int) + sizeof(struct  sockaddr_in));

        //接收客户端的请求链接 
        *(int *)cliAddr_socFd = accept(svrSocketFd,(struct sockaddr *)(cliAddr_socFd + sizeof(int)),&addrLength);
        if(*(int *)cliAddr_socFd < 0)
        {
            printf("--SERVER--:accept is error\r\n");
            return -1;
        }
        //创建线程
        ret = pthread_create(&thaId,NULL,Th_Login_Join,(void *)cliAddr_socFd);
        if(ret < 0)
        {
            printf("--SERVER--:pthread_create Th_Login_Join error!\n");
            return -1;
        }
        ret = pthread_detach(thaId);
        if(ret != 0)
        {
            printf("--SERVER--:pthread_detach Th_Login_Join error!\n");
        }
    }

	return 0;
}


void * Th_Login_Join(void * cliAddr_socFd)
{
    int ret;
    struct  sockaddr_in * cliAddrPtr;
    int * cliSocfdPtr;
    cliSocfdPtr = cliAddr_socFd;
    cliAddrPtr = cliAddr_socFd + sizeof(int);
    char recBuf[RECBUFSIZE];
    char sendBuf[SENDBUFSIZE];
    NODE * nodePtr;
    NODE newNode;

    while(1)    
    {
        memset(recBuf,0,RECBUFSIZE);
        ret = recv(*cliSocfdPtr, recBuf,RECBUFSIZE,0);    //MSG_DONTWAIT
        if(ret > 0)
        {
            if(strncmp(recBuf, "Login", LOGINHEAD) == 0)    //用户登入--------------------------------
            {
                nodePtr = Search_ByName(recBuf + LOGINHEAD);
                if(nodePtr != NULL && nodePtr->cliState == OFFLINE)
                {
                    nodePtr->cliState = ONLINE; //状态
                    nodePtr->cliSocketFd = *cliSocfdPtr; //fd
                    memset(sendBuf,0,SENDBUFSIZE);
                    sprintf(sendBuf,"Loginok%d",nodePtr->cliSocketFd);
                    ret = send(nodePtr->cliSocketFd,sendBuf,strlen(sendBuf) + 1,0);    //发送当前客户端fd
                    if(ret < 0)
                    {
                        printf("--SERVER--:send curCliSocketFd error!\n");
                        return ;
                    }
                    ret = Send_AllNode(nodePtr);
                    if(ret == 0)
                    {
                        printf("--SERVER--:Send_AllNode success!\n");
                    }
                    ret = Send_ClientState(nodePtr);
                    if(ret == 0)
                    {
                        printf("--SERVER--:Send_ClientState success!\n");
                    }
                    printf("--SERVER--:----------------------------- user %s has been online! -------------------------------\n",nodePtr->userName);
                    //打印客户端地址信息
                    printf("IP\t\t  PORT\tsocFd\tuserName\n");
                    printf("%s%8u%7d    %7s\n",inet_ntoa(cliAddrPtr->sin_addr), ntohs(cliAddrPtr->sin_port), nodePtr->cliSocketFd,nodePtr->userName);
                    //printf("--SERVER--:client IP:%s\n--SERVER--:client PORT:%u\n",inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port));
                    //打印客户端连接信息
                    //printf("--SERVER--:cliSocketFd:%5d | userName:%s\n",newNode.cliSocketFd,newNode.userName);
                    Display_ALL_User_Node();
                    free(cliAddr_socFd);
                    break;
                    
                }
                else
                {
                    memset(sendBuf,0,SENDBUFSIZE);
                    strcpy(sendBuf,"no such user");
                    ret = send(*cliSocfdPtr,sendBuf,strlen(sendBuf) + 1,0);    
                    if(ret < 0)
                    {
                        printf("--SERVER--:send Loginerror error!\n");
                        return ;
                    }
                }
            }
            else if(strncmp(recBuf, "Join", JOINHEAD) == 0)     //用户注册----------------------------
            {
                nodePtr = Search_ByName(recBuf + JOINHEAD);
                if(nodePtr == NULL)
                {
                    strcpy(newNode.userName,recBuf + JOINHEAD); //存取用户名
                    newNode.cliState = OFFLINE; //设置用户状态

                    /* 新建节点，成功：返回节点指针，失败：返回NULL */
                    nodePtr =  Create_Node(&newNode);
                    if(nodePtr == NULL)
                    {
                        printf("--SERVER--:Create_Node error!\n");
                        return ;
                    }

                    /* 将新节点插入链表末尾 */
                    Dlink_Insert_Last(nodePtr);

                    memset(sendBuf,0,SENDBUFSIZE);
                    strcpy(sendBuf,"Joinok");   
                    //sprintf(sendBuf + JOINHEAD + 2,"%d",*cliSocfdPtr);
                    ret = send(*cliSocfdPtr,sendBuf,strlen(sendBuf) + 1,0);   //发送注册成功应答 
                    if(ret < 0)
                    {
                        printf("--SERVER--:send Joinok error!\n");
                        return ;
                    }
                    ret = Send_NewNode();
                    if(ret = 0)
                    {
                        printf("--SERVER--:Send_NewNode success!\n");
                    }

                    printf("--SERVER--:--------------------------- The new user %s has been registered -------------------------------\n",recBuf + JOINHEAD);
                    printf("IP\t\t  PORT\t\tuserName\n");
                    printf("%s%8u     %13s\n",inet_ntoa(cliAddrPtr->sin_addr), ntohs(cliAddrPtr->sin_port), nodePtr->userName);
                    Display_ALL_User_Node();
                    close(*cliSocfdPtr);
                    free(cliAddr_socFd);
                    break;
                }
                else
                {
                    memset(sendBuf,0,SENDBUFSIZE);
                    strcpy(sendBuf,"Joinerror");
                    ret = send(*cliSocfdPtr,sendBuf,strlen(sendBuf) + 1,0);    
                    if(ret < 0)
                    {
                        printf("--SERVER--:send Joinerror error!\n");
                        return ;
                    }
                }
            }
            
            // else
            // {
            //     printf("--SERVER--:Failed to process registration and login requests!\n");
            //     break;
            //     //return -1;
            // }
        }
    }
}

//向在线用户发送新注册用户节点
int Send_NewNode(void)
{
    NODE * nodePtr,* lastNodePtr;
    int dLinkSize,count,ret;  //暂存双向链表的节点个数
    char sBuf[SENDBUFSIZE] = {0};
    char realLen;
    lastNodePtr = Get_Last_Node();
    dLinkSize = Dling_Size();
    nodePtr = Get_First_Node();
    count = 0;
    while(count < dLinkSize)
    {
        if(nodePtr->cliState == ONLINE)
        {
            memset(sBuf,0,SENDBUFSIZE);
            // sprintf(sBuf,"newcli%d",lastNodePtr->cliSocketFd);  //填入客户端fd
            // sprintf(sBuf + NEWCLIHEAD + CLIFDBUFSIZE,"%d",lastNodePtr->cliState);   //填入客户端状态
            // sprintf(sBuf + NEWCLIHEAD + CLIFDBUFSIZE + 1, "%s", lastNodePtr->userName);     //填入客户端账号
            // realLen = NEWCLIHEAD + CLIFDBUFSIZE +  1 + strlen(sBuf + NEWCLIHEAD + CLIFDBUFSIZE +  1) + 1;
            sprintf(sBuf,"newcli%d",lastNodePtr->cliState);  //填入客户端状态
            sprintf(sBuf + NEWCLIHEAD + 1,"%s",lastNodePtr->userName);   //填入客户端账号
            realLen = NEWCLIHEAD +  1 + strlen(sBuf + NEWCLIHEAD + 1) + 1;
            ret = send(nodePtr->cliSocketFd,sBuf,realLen, 0);
            if(ret < 0)
            {
                printf("--SERVER--:%s is error\n",__func__);
                return -1;
            }
        }
        count++;
        nodePtr = nodePtr->next;
    }

    return 0;
}

//向在线用户发送用户状态变更信息
int Send_ClientState(NODE * cliNode)
{
    int ret;
    NODE * nodePtr;
    int dLinkSize,count;  //暂存双向链表的节点个数
    char realLen;
    char sBuf[SENDBUFSIZE] = {0};  //用户标识符缓冲区
    memset(sBuf,0,SENDBUFSIZE);
    sprintf(sBuf,"state%d",cliNode->cliSocketFd);  //填入用户fd
    sprintf(sBuf + STATEHEAD + CLIFDBUFSIZE,"%u",cliNode->cliState);  //填入状态信息
    sprintf(sBuf + STATEHEAD + CLIFDBUFSIZE + 1,"%s",cliNode->userName);
    dLinkSize = Dling_Size();
    nodePtr = Get_First_Node();
    count = 0;
    realLen = STATEHEAD + CLIFDBUFSIZE + 1 + strlen(sBuf + STATEHEAD + CLIFDBUFSIZE + 1) + 1;
    while(count < dLinkSize)
    {
        if(nodePtr->cliState == ONLINE)
        {
            ret = send(nodePtr->cliSocketFd,sBuf,realLen, 0);
            if(ret < 0)
            {
                printf("--SERVER--:%s is error\n",__func__);
                return -1;
            }
        }
        count++;
        nodePtr = nodePtr->next;
    }

    return 0;
}
//向当前用户发送所有用户信息
int Send_AllNode(NODE * curNode)
{
    int ret;
    NODE * nodePtr;
    int dLinkSize,count;  //暂存双向链表的节点个数
    char realLen;
    char sBuf[SENDBUFSIZE] = {0};
    dLinkSize = Dling_Size();
    nodePtr = Get_First_Node();
    count = 0;
    while(count < dLinkSize)
    {
        //printf("fd = %d,name = %s,state = %u\n",nodePtr->cliSocketFd,nodePtr->userName,nodePtr->cliState);      //////////////////////////////
        memset(sBuf,0,SENDBUFSIZE);
        sprintf(sBuf,"allcli%d",nodePtr->cliSocketFd);  //填入客户端fd
        sprintf(sBuf + ALLCLIHEAD + CLIFDBUFSIZE,"%u",nodePtr->cliState);   //填入客户端状态
        sprintf(sBuf + ALLCLIHEAD + CLIFDBUFSIZE + 1, "%s", nodePtr->userName);     //填入客户端账号
        realLen = ALLCLIHEAD + CLIFDBUFSIZE +  1 + strlen(sBuf + ALLCLIHEAD + CLIFDBUFSIZE +  1) + 1;
        ret = send(curNode->cliSocketFd,sBuf,realLen, 0);
        if(ret < 0)
        {
            printf("--SERVER--:%s is error\n",__func__);
            return -1;
        }
        count++;
        nodePtr = nodePtr->next;
        usleep(10000);
    }
    strcpy(sBuf,"end");
    ret = send(curNode->cliSocketFd,sBuf,strlen(sBuf) + 1,0);
    if(ret < 0)
    {
        printf("--SERVER--:send end error\n");
        return -1;
    }
    usleep(10000);
    return 0;
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

void Display_ALL_User_Node(void)
{
    int i,count,dLinkSize;
    NODE * nodePtr;
    dLinkSize = Dling_Size();
    i = 0;
    count = 0;
    printf("========== user node list  =========\n");
    if(dLinkSize)
    {   
        nodePtr = Get_First_Node();
        while(i < dLinkSize)
        {
            if(nodePtr->cliState == ONLINE)
            {
                printf("%15s%6d%2d\n",nodePtr->userName,nodePtr->cliSocketFd,nodePtr->cliState);
            }
            else
            {
                printf("%15s      %2d\n",nodePtr->userName,nodePtr->cliState);
            }
            i++;
            nodePtr = nodePtr->next;
        }
    }
    printf("======================================\n");
    printf("total numer:%d\n",dLinkSize);
    
}