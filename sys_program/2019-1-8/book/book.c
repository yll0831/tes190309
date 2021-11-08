#include <stdio.h>
#include "book.h"


void inputBookInfo(SBook * p)
{
	if (NULL == p)
	{
		return ;
	}
	printf("input book num and price:");
	scanf("%d", &(p->num));
	scanf("%f", &(p->price));
}


void showBookInfo(const SBook * p)
{
	if (NULL == p)
	{
		return ;
	}

	printf("total num=%d, price=%.1f\r\n", p->num, p->price);
}
