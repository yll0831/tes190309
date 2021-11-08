OBJ := obj/main.o
OBJ += obj/fun1.o
OBJ += obj/fun2.o
CC = gcc

bin/funexcu:$(OBJ)
	@$(CC) -o $@ $(OBJ)

obj/main.o:
	@make -C src/

obj/fun1.o:
	@make -C src/fun1/

obj/fun2.o:
	@make -C src/fun2/

.PHONY:clean

clean:
	@rm -rf bin/funexcu obj/*.o
	
