#include"TcpSocket.h"

TcpServer::TcpServer(){
    m_listenfd = -1;
    m_clientfd = -1;
    memset(&serverAddr, 0, sizeof(serverAddr));
    memset(&clientAddr, 0, sizeof(clientAddr));
}

TcpServer::~TcpServer(){
    if(m_listenfd > 0){
        close(m_listenfd);
    }
    if(m_clientfd > 0){
        close(m_clientfd);
    }
}

bool TcpServer::initServer(const unsigned short port){
    if((m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0){
        return false;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);
    if(bind(m_listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0){
        close(m_listenfd);
        return false;
    }
    if(listen(m_listenfd, 128) != 0){
        close(m_listenfd);
        return false;
    }
    return true;
}

bool TcpServer::acceptConn(const int& listenfd){
    if(listenfd == -1){
        return false;
    }
    socklen_t len = sizeof(clientAddr);
    if((m_clientfd = accept(listenfd, (struct sockaddr*)&clientAddr, &len)) < 0){
        return false;
    }
    return true;
}

char* TcpServer::getClientIP(){
    return inet_ntoa(clientAddr.sin_addr);
}

string TcpServer::recvMsg(){
    int len = 0;
    readn(m_clientfd, (char*)&len, 4);
    len = ntohl(len);
    //cout << "数据块大小：" << len << endl;

    char* buf = new char[len + 1];
    readn(m_clientfd, buf, len);
    buf[len] = '\0';
    string str(buf);
    delete[] buf;
    return str;
}

int TcpServer::sendMsg(string& msg){
    char* data = new char[msg.size()+4];
    int headLen = htonl(msg.size());
    memcpy(data, (char*)&headLen, 4);
    memcpy(data+4, msg.c_str(), msg.size());
    int len = writen(m_clientfd, data, msg.size()+4);
    delete[] data;
    return len;
}


TcpClient::TcpClient(){
    m_clientfd = -1;
}

TcpClient::~TcpClient(){
    closeConn();
}

bool TcpClient::connectToHost(string ip, const unsigned short port){
    if(m_clientfd != -1){
        close(m_clientfd);
        m_clientfd = -1;
    }
    m_clientfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr= inet_addr(ip.c_str());
    clientAddr.sin_port = htons(port);

    if((connect(m_clientfd, (struct sockaddr*)&clientAddr, sizeof(clientAddr))) != 0){
        close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
    return true;
}

string TcpClient::recvMsg(){
    int len = 0;
    readn(m_clientfd, (char*)&len, 4);
    len = ntohl(len);
    cout << "数据块大小：" << len << endl;

    char* buf = new char[len + 1];
    readn(m_clientfd, buf, len);
    buf[len] = '\0';
    string str(buf);
    delete[] buf;
    return str;
}

int TcpClient::sendMsg(string& msg){
    char* data = new char[msg.size()+4];
    int headLen = htonl(msg.size());
    memcpy(data, (char*)&headLen, 4);
    memcpy(data+4, msg.c_str(), msg.size());
    int len = writen(m_clientfd, data, msg.size()+4);
    delete[] data;
    return len;
}

void TcpClient::closeConn(){
    if(m_clientfd > 0){
        close(m_clientfd);
    }
    m_clientfd = -1;
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