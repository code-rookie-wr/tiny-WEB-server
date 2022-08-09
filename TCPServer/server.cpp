#include"TcpSocket.h"

void working(void* arg){
    TcpServer* client = (TcpServer*)arg;
    char* ip = client->getClientIP();
    cout << "Client IP: " << ip << endl;
    while(1){
        string msg = client->recvMsg();
        if(!msg.empty()){
            cout << msg << "\tThread ID: " << this_thread::get_id() << endl;
        }
        else{
            break;
        }
    }
    delete client;
}

int main(){
    TcpServer server;
    if(server.initServer(9999) == false){
        cerr << "init failed" << endl;
    }
    cout << "waiting for connection ..." << endl;
    while(1){
        TcpServer* client = new TcpServer;
        if(client->acceptConn(server.m_listenfd) == false){
            cerr << "accept failed" << endl;
            break;
        }
        thread th(working, client);
        th.detach();
    }
    return 0;
}