LITAPPS=stdio stdio1 stdio2 stdio3 stdio4 stdio5 stdio6 stdio7 stdio8 \
		lock1 lock2 lseek read append dup printf open tree testlock stat \
		opendir fmtwrite fwrite lock3 split compose nonblock bufferio ncurses \
		serialio
CFLAGS = -g
CC = gcc

all: $(LITAPPS)

compose: compose.o
split: split.o
stdio7: stdio7.o filelock.o
stdio8: stdio8.o filelock.o
lock1: filelock.o lock1.o
lock2: filelock.o lock2.o
	gcc filelock.o lock2.o -o lock2 -lpthread
lock3: filelock.o lock3.o

ncurses: ncurses.o
	gcc ncurses.o -lncurses -o ncurses
serialio: serialio.h
.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@echo "clean..."
	rm *.o 
	rm $(LITAPPS)

