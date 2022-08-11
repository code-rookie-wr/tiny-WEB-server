#include"../TCPServer/TcpSocket.h"
#include<sys/epoll.h>
#include<thread>
#include<errno.h>
#include<fcntl.h>


int main(){
    TcpServer server;
    if(server.initServer(9999) == false){
        cerr << "init failed" << endl;
        return -1;
    }
    cout << "waiting for connection ..." << endl;

    //创建epoll模型
    int epfd = epoll_create(1);
    if(epfd == -1){
        perror("epoll_create");
        return -1;
    }

    //创建事件，向epoll中添加需要检测的的节点
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server.m_listenfd;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, server.m_listenfd, &ev) == -1){
        perror("epoll_ctl");
    }

    struct epoll_event evs[1024];
    int evsize = sizeof(evs)/sizeof(struct epoll_event);

    while(1){
        int evnum = epoll_wait(epfd, evs, evsize , -1);
        for(int i = 0; i < evnum; i++){
            //取出当前的文件描述符
            int curfd = evs[i].data.fd;
            if(curfd == server.m_listenfd){
                //建立新连接
                int clientfd = accept(curfd, NULL, NULL);
                //将文件描述符改为非阻塞
                int flag = fcntl(clientfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(clientfd, F_SETFL, flag);
                //得到的新文件描述符添加到epoll模型中
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientfd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev) == -1){
                    perror("epoll_ctl");
                    break;
                }
            }
            else{
                while(1){
                    string msg = recvMsg(curfd);
                    if(msg.empty()){
                        //出现这个错误，说明一次内发送的数据已经接收完
                        if(errno == EAGAIN){
                            break;
                        }
                        else{
                            cerr << "客户端已经断开连接" << endl;
                            epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                            close(curfd);
                            break;
                        }
                    }
                    else{
                        cout << msg << "\tThread ID: " << this_thread::get_id() << endl;
                    }
                }
            }
        }
    }


    return 0;
}