#include "../TCPServer/TcpSocket.h"
#include <thread>


int main(){
    TcpClient client;
    if(client.connectToHost("127.0.0.1", 9999) == false){
        return -1;
    }
    cout << "成功连接到服务端 ..." << endl;
    for(int i = 0; i < 100; i++){
        string str = "hello world";
        cout << str << "\tThread ID: " << this_thread::get_id() << endl;
        sendMsg(client.m_clientfd, str);
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    return 0;
}

