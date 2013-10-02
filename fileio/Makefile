LITAPPS=stdio stdio1 stdio2 stdio3 stdio4 stdio5 stdio6 stdio7 stdio8 \
		lock1 lock2 lseek read append dup printf open tree testlock stat \
		opendir fmtwrite fwrite lock3 split compose nonblock bufferio ncurses 

CFLAGS = -g 
CC = gcc
PLAT_FORM = MAC_OS

all: $(LITAPPS)

stdio7: filelock.o
stdio8: filelock.o
lock1: filelock.o lock1.o
lock2: filelock.o lock2.o 
	gcc $^ -o $@ -lpthread
lock3: filelock.o lock3.o

ncurses: ncurses.o
	gcc $^ -o $@ -lncurses 

# mac os doesn't support stropts.h
ifneq ($(PLAT_FORM), MAC_OS)
serialio: serialio.h
endif
.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@echo "clean..."
	-rm *.o $(LITAPPS)
	-rm -rf *.dSYM

