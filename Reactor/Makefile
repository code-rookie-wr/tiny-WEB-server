SOURCE := Channel.cc Epoll.cc EventLoop.cc ../TCPServer/TcpSocket.cpp
CC := g++
LIBS := -pthread -std=c++11
TARGET := server client

all : $(TARGET)

server : $(SOURCE)
	$(CC) $^ server.cc -o $@ $(LIBS)

client : $(SOURCE)
	$(CC) $^ client.cc -o $@ $(LIBS)

.PHONY:
clearall:
	rm -rf $(OBJS) $(TARGET)
clear:
	rm -rf $(OBJS)
