target =cli_chat.o
target +=Double_Link.o
prerequisites=cli_chat.c
prerequisites+=Double_Link.c
$(target):$(prerequisites)
	@gcc -c cli_chat.c -o ../object/cli_chat.o -I ../include -pthread
	@gcc -c Double_Link.c -o ../object/Double_Link.o -I ../include -pthread