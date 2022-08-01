#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cctype>
#include<cstring>

using namespace std;

constexpr int MAXBUFLEN = 128; //最大接收长度
constexpr const char* closeKey = "CLOSESERVER"; //提供一个客户端关闭服务器的接口

int main(int argc, char* argv[]){

    if(argc != 2){
        cerr << "Usage: ./server.exe port" << endl;
        exit(-1);
    }

    sockaddr_in serverAddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    int bindRet = bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if(bindRet != 0){
        cerr << "bind failed !" << endl;
        close(sockfd);
        exit(-2);
    }

    int listenRet = listen(sockfd, 16);
    if(bindRet != 0){
        cerr << "listen failed !" << endl;
        close(sockfd);
        exit(-3);
    }

    cout << "Wating for connection..." << endl;

    while(1){
        sockaddr_in clientAddr;

        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientfd = accept(sockfd, (sockaddr*)&clientAddr, &clientAddrLen);
        if(clientfd < 0) {
            cerr << "accept failed! " << endl;
            close(sockfd);
            close(clientfd);
            exit(-4);
        }

        cout << "Client IP : " << inet_ntoa(clientAddr.sin_addr) << "\tport :" << ntohs(clientAddr.sin_port) << endl;
        
        char* buffer = new char[MAXBUFLEN];
        int recvBytes = recv(clientfd, buffer, MAXBUFLEN, 0);
        if(recvBytes > 0){
            //cout << buffer << endl;
            //当客户端输入指定字符串可关闭服务器
            if(strcmp(buffer, closeKey) == 0){
                close(clientfd);
                close(sockfd);
                exit(0);
            }
            //将接收的数据转成大写
            for(int i = 0; i < recvBytes; i++){
                buffer[i] = toupper(buffer[i]);
            }
            send(clientfd, buffer, strlen(buffer), 0);
        }
        delete[] buffer;
        close(clientfd);
    }   
    close(sockfd);
    return 0;
}