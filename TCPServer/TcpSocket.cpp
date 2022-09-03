#include"TcpSocket.h"

TcpServer::TcpServer(){
    _listenfd = -1;
    _clientfd = -1;
    memset(&_serverAddr, 0, sizeof(_serverAddr));
    memset(&_clientAddr, 0, sizeof(_clientAddr));
}

TcpServer::~TcpServer(){
    if(_listenfd > 0){
        close(_listenfd);
    }
    if(_clientfd > 0){
        close(_clientfd);
    }
}

bool TcpServer::initServer(const unsigned short port){
    if((_listenfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
        return false;
    }
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAddr.sin_port = htons(port);
    if(bind(_listenfd, (struct sockaddr*)&_serverAddr, sizeof(_serverAddr)) != 0){
        perror("bind");
        close(_listenfd);
        return false;
    }
    if(listen(_listenfd, 128) != 0){
        perror("listen");
        close(_listenfd);
        return false;
    }
    return true;
}

bool TcpServer::acceptConn(const int& listenfd){
    if(listenfd == -1){
        return false;
    }
    socklen_t len = sizeof(_clientAddr);
    if((_clientfd = accept(listenfd, (struct sockaddr*)&_clientAddr, &len)) < 0){
        perror("accept");
        return false;
    }
    return true;
}

char* TcpServer::getClientIP(){
    return inet_ntoa(_clientAddr.sin_addr);
}

void TcpServer::setNoBlocking(int& fd){
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}



TcpClient::TcpClient(){
    _clientfd = -1;
}

TcpClient::~TcpClient(){
    closeConn();
}

bool TcpClient::connectToHost(string ip, const unsigned short port){
    if(_clientfd != -1){
        close(_clientfd);
        _clientfd = -1;
    }
    _clientfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr= inet_addr(ip.c_str());
    clientAddr.sin_port = htons(port);

    if((connect(_clientfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr))) != 0){
        perror("connect");
        close(_clientfd);
        _clientfd = -1;
        return false;
    }
    return true;
}

void TcpClient::closeConn(){
    if(_clientfd > 0){
        close(_clientfd);
    }
    _clientfd = -1;
}


string recvMsg(const int& fd){
    int len = 0;
    readn(fd, (char*)&len, 4);
    len = ntohl(len);
    //cout << "数据块大小：" << len << endl;

    char* buf = new char[len + 1];
    readn(fd, buf, len);
    buf[len] = '\0';
    string str(buf);
    delete[] buf;
    return str;
}

int sendMsg(const int& fd, string& msg){
    char* data = new char[msg.size()+4];
    int headLen = htonl(msg.size());
    memcpy(data, (char*)&headLen, 4);
    memcpy(data+4, msg.c_str(), msg.size());
    int len = writen(fd, data, msg.size()+4);
    delete[] data;
    return len;
}

int readn(const int& sockfd, char* buf, int size){
    int len = 0;
    int sizeLeft = size;
    int index = 0;
    while(sizeLeft > 0){
        if((len = recv(sockfd, buf+index, sizeLeft, 0)) <= 0){
            return -1;
        }
        sizeLeft -= len;
        index += len;
    }
    return size;
}

int writen(const int& sockfd, const char* buf, int size){
    int len = 0;
    int sizeLeft = size;
    int index = 0;
    while(sizeLeft > 0){
        if((len = send(sockfd, buf+index, sizeLeft, 0)) <= 0){
            return -1;
        }
        sizeLeft -= len;
        index += len;
    }
    return size;
}