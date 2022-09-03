#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>


using namespace std;

class TcpServer{
public:
    //构造函数
    TcpServer();
    //析构函数
    ~TcpServer();
    //服务端初始化
    bool initServer(const unsigned short port);
    //阻塞等待客户端的连接请求
    bool acceptConn(const int& listenfd);
    //获取客户端ip
    char* getClientIP();
    //设置非阻塞
    void setNoBlocking(int& fd);

public:
    int _listenfd;  //服务端监听的fd
    int _clientfd;  //客户端连接上来的fd

private:
    struct sockaddr_in _serverAddr;  //服务端地址信息
    struct sockaddr_in _clientAddr;  //客户端地址信息
};


class TcpClient{
public:
    //构造函数
    TcpClient();
    //析构函数
    ~TcpClient();
    //向服务端发起连接请求
    bool connectToHost(string ip, const unsigned short port);
    //接收服务端数据
    string recvMsg();
    //向服务端发送数据
    int sendMsg(string& msg);
    //与服务端断开连接
    void closeConn();

public:
    int _clientfd;  //客户端的fd
};


//接收数据
string recvMsg(const int& fd);
//发送数据
int sendMsg(const int& fd, string& msg);

int readn(const int& sockfd, char* buf, int size);
int writen(const int& sockfd, const char* buf, int size);

#endif 