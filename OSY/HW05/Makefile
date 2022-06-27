all:prod-cons
prod-cons: threadcontrol.c list.c list.h
	gcc -Wall -g -pthread threadcontrol.c list.c list.h -o prod-cons
	
test: threadcontrol.c list.c list.h
	gcc -Wall -g -pthread -fsanitize=address threadcontrol.c list.c list.h -o prod-cons	