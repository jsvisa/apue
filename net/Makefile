TARGET=getindent connect bind select stdio server msend mrecv \
	   udpserver udpclient accept singlechat broadcast inetaddr \
	   simptcpcli simptcpserv selectserv telnet poll \
	   echotime echotimecli lunxun shutdown  setsockopt \
	   sendudp recvfrom setsockbuf udptest
all:$(TARGET)
CFLAGS=-g 
CXXFLAGS=-g -lpthread
LDFLAGS=-lpthread
CC=gcc

connect:connect.o  
bind:bind.o csocket.o 
select:csocket.o select.o
server:server.o csocket.o
msend:msend.o csocket.o
mrecv:mrecv.o 
accept:accept.o csocket.o
singlechat:singlechat.o csocket.o
selectserv:selectserv.o csocket.o
#testclient:testclient.o csocket.o
shutdown:shutdown.o csocket.o
poll:poll.o csocket.o
setsockopt:setsockopt.o csocket.o
gethostbyname:gethostbyname.o csocket.o
sendudp:sendudp.o csocket.o

clean:
	-rm -rf *.dSYM 
	-rm *.o $(TARGET) 
