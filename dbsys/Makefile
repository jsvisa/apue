LITAPPS=dbsys
CFLAGS = -g
CC = gcc

all: $(LITAPPS)

dbsys: dbsys.o main.o shellsys.o crc16.o filelock.o 
.c.o:
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	@echo "clean..."
	rm *.o -f
	rm $(LITAPPS)
