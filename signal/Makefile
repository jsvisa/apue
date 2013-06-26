TARGET=signal sigchld div0 sig_def_ign sig_kill_stop term_int sig_fork sig_default sig_resethandle\
	   sigaction sig_restart sig_reent nonentry kill sigprocmask sigpending \
	   sigint sigsuspend sigstop siginfo sigqueue signal_term_int 
CFLAGS=-g
CC=gcc

all:$(TARGET)
signal:signal.c
	$(CC) $(CFLAGS) signal.c -o signal
clean:
	rm *.o -f
	rm $(TARGET) -f
