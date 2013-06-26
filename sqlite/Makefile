TARGET=demo sqlstep sqlite_bind sqlite_open
all:$(TARGET)
CFLAGS=-g -I/usr/local/include/
CXXFLAGS=-g -I/usr/local/include/

LDFLAGS= -lsqlite3 -lpthread
CC=g++

demo:demo.o msqlite.o
sqlite_bind:sqlite_bind.o

clean:
	@echo "Cleaning..."
	@rm *.o -f
	@rm $(TARGET) -f

