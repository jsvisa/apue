src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

CC=g++
CXXFLAGS=-g
LDFLAGS=-lpthread

rtspserver: $(obj)

.PHONY: clean
clean:
	-rm *.o rtspserver 
