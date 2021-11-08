#ifndef _BOOK_H_
#define _BOOK_H_


#define COUNT 3

struct sbook{
	int num;
	float price;
};

typedef struct sbook SBook;

void inputBookInfo(SBook * p);
void showBookInfo(const SBook * p);

#endif // _BOOK_H_
