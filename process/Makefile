LITAPPS=echoall env execv execve execvp _exit fork_bufio fork putenv \
		setenv setjmp vfork wait atexit getenv getpid fork1 fork2 setjmp1 \
		system exit stack execl
CFLAGS = -g
CC = gcc

all: $(LITAPPS)

.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@echo "clean..."
	-rm -f *.o
	-rm -f $(LITAPPS)
#	cd /usr/local; pwd
