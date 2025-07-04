default: 
	@gcc main.c specification1.c functions.c specification26.c specification3.c specification4.c specification5.c specification7.c specification8.c
	@./a.out

debug: 
	@gcc main.c specification1.c functions.c specification26.c specification3.c specification4.c specification5.c specification7.c specification8.c -g
	@gdb ./a.out