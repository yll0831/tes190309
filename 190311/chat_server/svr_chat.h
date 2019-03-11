#ifndef SVR_CHAT_H
#define SVR_CHAT_H



int Send_NewNode(void);
int Send_ClientState(NODE * cliNode);
int Send_AllNode(NODE * curNode);
NODE * Search_ByName(char * uName);
void * Th_Login_Join(void * cliAddr_socFd);
void Display_ALL_User_Node(void);








#endif