TARGETS=pipe popen mkfifo readfifo writefifo mmap shmex wshm rshm\
	   addr mmap2 unixsockserv unixsockclient semserv semclient\
	   my_pipe #mmap_client

all:$(TARGETS)
CFLAGS=-g
CC=gcc

clean:
	rm *.o -f
	rm $(TARGETS) -f

