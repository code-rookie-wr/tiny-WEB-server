#include <iostream>
#include "../TCPServer/TcpSocket.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"
#include <thread>
#include <errno.h>

using namespace std;

shared_ptr<EventLoop> loop = make_shared<EventLoop>();

void working(shared_ptr<Channel>& client){
    while(1){
        string msg = recvMsg(client->getfd());
        if(!msg.empty()){
            cout << msg << "\tThread ID: " << this_thread::get_id() << endl;
        }
        else{
            if(errno == EAGAIN){
                break;
            }
            else{
                cerr << "客户端已经断开连接" << endl;
                loop->pollerDel(client);
                break;
            }
        }
    }
}

int main(){
    TcpServer server;
    if(server.initServer(9999) == false){
        return -1;
    }
    cout << "waiting for connection ..." << endl;
    shared_ptr<Channel> acceptChannel = make_shared<Channel>(server._listenfd);
    acceptChannel->setReadHandler([&](){
        TcpServer* client = new TcpServer;
        if(client->acceptConn(server._listenfd) == false){
            exit(-1);
        }
        client->setNoBlocking(client->_clientfd);
        shared_ptr<Channel> readChannel = make_shared<Channel>(client->_clientfd);
        readChannel->setReadHandler(bind(working, readChannel));
        loop->pollerAdd(readChannel);
    });
    loop->pollerAdd(acceptChannel);
    loop->loop(4);
    return 0;
}