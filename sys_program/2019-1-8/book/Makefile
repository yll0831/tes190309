OBJ:= main.o book.o
OBJ+=menu.o
CC=gcc

s:$(OBJ)
	$(CC) -o $@ $^
	
main.o:main.c
	$(CC) -c $^ -I include 
		
book.o:book.c
	$(CC) -c $^
	
menu.o:
	make -C  menu 

.PHONY:clean

clean:
	rm -rf *.o s *.h.gch

