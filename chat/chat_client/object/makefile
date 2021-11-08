
target=user_1
target+=user_2
target+=user_3
target+=user_4
target+=user_5
target+=user_6
prerequisites =cli_chat.o
prerequisites +=Double_Link.o
$(target):$(prerequisites)
	@gcc $^ -o ../bin/user_1	-pthread
	@gcc $^ -o ../bin/user_2	-pthread
	@gcc $^ -o ../bin/user_3	-pthread
	@gcc $^ -o ../bin/user_4	-pthread
	@gcc $^ -o ../bin/user_5	-pthread
	@gcc $^ -o ../bin/user_6	-pthread