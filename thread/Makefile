TARGET=thread_sig first_thread thread_arg pthread_join pthread_detach pthread_attr_detach \
	   pthread_mutex thread_rwlock pthread_cond_signal max_thread cond_wait max_thread1 max_thread2 \
	   max_thread3 max_thread4 threadsig mem_share open_thread pthread_mutex_pshared \
	   thread pthread_create pthread_cancel
all:$(TARGET)
CFLAGS=-g
CXXFLAGS=-g
LDFLAGS=-lpthread

CC=g++
CXX=g++

thread_sig:thread_sig.o
thread:thread.o csocket.o

clean:
	rm *.o -f
	rm $(TARGET) -f
