EXTRA_CFLAGS?=-fsanitize=address -fno-omit-frame-pointer
all: factory
factory: main.c list.c list.h
	gcc main.c list.c list.h -Wall -g -O2 -pthread $(EXTRA_CFLAGS) -o factory
