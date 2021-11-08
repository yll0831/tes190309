#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Double_Link.h"






/* 表头不存放元素值 */
NODE * phead = NULL;


/* 节点个数 */
int Node_Count = 0;

/* 新建节点，成功：返回节点指针，失败：返回NULL */
NODE * Create_Node(NODE * newNode)
{
	NODE * pnode = NULL;
	
	pnode = (NODE *)malloc(sizeof(NODE));	
	
	/* 节点创建失败 */
	if(!pnode)
	{
		printf("Create node error!\r\n");
		return NULL;
	}
	
	/* 初始化新创建的节点 */	
	pnode->prev = pnode;
	pnode->next = pnode;
    strcpy(pnode->userName,newNode->userName);
    pnode->cliSocketFd = newNode->cliSocketFd;
    pnode->cliState = newNode->cliState;
	
	return pnode;
}

/* 新建双向链表，成功：返回 0，失败：返回 -1 */
int Create_Dlink()
{
    NODE headNode;
	phead = Create_Node(&headNode);
	
	/* 双向链表创建失败 */
	if(!phead)
	{
		printf("Create Dlink error!\r\n");	
		return -1;
	}
	
	Node_Count = 0;
	
	return 0;
}

/* 双向链表是否为空，是，返回：1，否，返回：0 */
int Dlink_is_Empty()
{
	return Node_Count == 0;
}


/* 获取双向链表的节点个数 */
int Dling_Size()
{
	return Node_Count;
}

/* 获取双向链表中index位置的节点（节点序号从 0 开始） */
NODE * Get_Node(int index)
{
	/* 所取节点序号超出范围 */
	if(index < 0 || index >= Node_Count)
	{
		printf("%s failed!index of bound!\r\n",__func__);
		return NULL;
	}
	
	int i = 0;//节点序号
	NODE * pnode = phead;
	
	/* 正向查找 */
	if(index <= (Node_Count / 2))
	{
		pnode = pnode->next;
		while(i++ < index )
			pnode = pnode->next;
		
		return pnode;
	}
	
	i = 0;
	
	/* 反向查找 */
	int pindex = Node_Count - index - 1;
	
	pnode = pnode->prev;
	while(i++ < pindex)
		pnode = pnode->prev;
		
	return pnode;
}


/* 获取第一个节点 */
NODE * Get_First_Node()
{
	return Get_Node(0);
} 

/* 获取最后一个节点 */
NODE * Get_Last_Node()
{
	return Get_Node(Node_Count - 1);
}

/* 将新节点插入链表表头 */
int Dlink_Insert_First(NODE * pnode)
{
	pnode->next = phead->next;
	pnode->prev = phead;
	pnode->next->prev = pnode;
	phead->next = pnode;
	
	Node_Count++;
	
	return 0;
}


/* 将新节点插入链表末尾 */
int Dlink_Insert_Last(NODE * pnode)
{
	pnode->prev = phead->prev;
	pnode->next = phead;
	pnode->prev->next = pnode;
	phead->prev = pnode;
	
	Node_Count++;
	
	return 0;
}

/* 将新节点插入到 index 位置，成功：返回 0 ，失败，返回 -1 */
int Dling_Insert(int index,NODE * pnode)
{
	/* 插入表头 */
	if(index == 0)
	{
		return Dlink_Insert_First(pnode);
	}
	
	/* 获取index处的节点 */
	NODE * pindex = Get_Node(index);
	/* 获取index节点失败 */
	if(!pindex)
	{
		return -1; 
	}
	
	/* 插入节点 */
	pnode->next = pindex;
	pnode->prev = pindex->prev;
	pindex->prev->next = pnode;
	pindex->next->prev = pnode;
	
	/* 节点个数加 1 */
	Node_Count++;
	
	return 0;
}

/* 删除双向链表中 index 位置的节点，成功，返回：0，失败，返回： -1 */
int Dlink_Delete(int index)
{
	NODE * pindex = Get_Node(index);
	
	/* index 节点超出链表范围 */
	if(!pindex)
	{
		printf("%s failed!the index is out of bound!\r\n",__func__);
		return -1;
	}
	
	/* 删除节点 */
	pindex->prev->next = pindex->next;
	pindex->next->prev = pindex->prev;
	/* 释放节点 */
	free(pindex);
	
	/* 节点个数减 1 */
	Node_Count--;
	
	return 0;
}

/* 删除第一个节点 */
int Dlink_Delete_First()
{
	return Dlink_Delete(0);
}

/* 删除最后一个节点 */
int Dlink_Delete_Last()
{
	return Dlink_Delete(Node_Count - 1);
}

/* 撤销 双向链表 ，成功，返回 0,失败，返回 -1 */
int Destroy_Dlink()
{
	if(!phead)
	{
		printf("%s failed!Dlink is NULL!\r\n",__func__);
		return -1;
	}
	
	NODE * pnode = phead->next;
	NODE * ptmp = NULL;
	while(pnode != phead)
	{
		ptmp = pnode;
		pnode = pnode->next;
		free(ptmp);
	}
	
	free(phead);
	
	phead = NULL;
	
	Node_Count = 0;
	
	return 0;
}











