#include <stdio.h>
#include "book.h"
#include "menu.h"

int main()
{
	SBook bookArray[COUNT] = {};
	int inputNum = 0;
	int i = 0;
	
	while (1){
	menu();
	scanf("%d", &inputNum);
	switch(inputNum)
	{
	case INPUTBOOKINFO : 
		inputBookInfo(bookArray+i);
		i++;
		if (COUNT == i)
		{
			i = 0;
		}
		break;	
	case SHOWBOOKINFO:
		showBookInfo(bookArray+0);
		break;
	case QUIT:
		return 0;
	default:
		break;
	}	
	}
	printf("end\r\n");
	return 0;
}
