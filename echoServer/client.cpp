#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>

using namespace std;

constexpr int MAXBUFLEN = 128; //最大接收长度

int main(int argc, char* argv[]){
    if(argc != 3){
        cerr << "Usage: ./client port message" << endl;
        exit(-1);
    }

    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(atoi(argv[1]));

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    int connectRet = connect(clientfd, (sockaddr*)&clientAddr, sizeof(clientAddr));
    if(connectRet == -1){
        perror("connect failed");
        close(clientfd);
        exit(-2);
    }

    send(clientfd, argv[2], strlen(argv[2]), 0);

    char* buffer = new char[MAXBUFLEN];
    int recvBytes = recv(clientfd, buffer, MAXBUFLEN, 0);
    if(recvBytes > 0){
        cout << "Receive[" << recvBytes << "]: " << buffer << endl;
    }

    cout << "finished" << endl;
    close(clientfd);
    
    return 0;
}