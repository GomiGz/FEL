all: forkpipe nsd
nsd:nsd_main.c nsd.c nd.c nsd.h nd.h
	gcc nsd_main.c nsd.c nd.c nsd.h nd.h -Wall -I. -o nsd
forkpipe:main.c generator.c
	gcc $^ -Wall -o $@
