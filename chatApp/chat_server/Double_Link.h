#ifndef DOUBLE_LINK_H
#define DOUBLE_LINK_H

#define USERNAMESIZE 15

//双向链表节点
typedef struct node
{
	struct node * prev;
	struct node * next;
	char userName[USERNAMESIZE + 1];    //用户名
    int cliSocketFd;    //用户fd
    unsigned char cliState:1;       //用户状态
}NODE;

/* 外部声明 */
extern NODE * phead;
extern int Node_Count;

/* 新建节点，成功：返回节点指针，失败：返回NULL */
NODE * Create_Node(NODE * newNode);

/* 新建双向链表，成功：返回 0，失败：返回 -1 */
int Create_Dlink();

/* 双向链表是否为空，是，返回：1，否，返回：0 */
int Dlink_is_Empty();

/* 获取双向链表的节点个数 */
int Dling_Size();

/* 获取双向链表中index位置的节点（节点序号从 0 开始） */
NODE * Get_Node(int index);

/* 获取第一个节点 */
NODE * Get_First_Node();

/* 获取最后一个节点 */
NODE * Get_Last_Node();

/* 将新节点插入链表表头 */
int Dlink_Insert_First(NODE * pnode);

/* 将新节点插入链表末尾 */
int Dlink_Insert_Last(NODE * pnode);

/* 将新节点插入到 index 位置，成功：返回 0 ，失败，返回 -1 */
int Dling_Insert(int index,NODE * pnode);

/* 删除双向链表中 index 位置的节点，成功，返回：0，失败，返回： -1 */
int Dlink_Delete(int index);

/* 删除第一个节点 */
int Dlink_Delete_First();

/* 删除最后一个节点 */
int Dlink_Delete_Last();

/* 撤销 双向链表 ，成功，返回 0,失败，返回 -1 */
int Destroy_Dlink();














#endif

