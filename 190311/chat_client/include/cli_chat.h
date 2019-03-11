#ifndef CLI_CHAT_H
#define CLI_CHAT_H



void Display_Friend_List(void);
int Init_Client(void);
int Join(void);
int Login(void);
void Send_OffLineSignal(int svrSocketFd);
int Conect_Server(void);
NODE * Search_BySocketFd(int cliSocketFd);
NODE * Search_ByName(char * uName);







#endif