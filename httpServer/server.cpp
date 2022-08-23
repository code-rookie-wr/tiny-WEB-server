#include"../TCPServer/TcpSocket.h"
#include"errorProcess.h"
#include<thread>
#include<sys/epoll.h>

void doHTTPRequest(void* arg);
void doHTTPResponse(const int& fd, const string path);
int getLine(const int& fd, string& buf);

int epfd = -1;

int main(){
    TcpServer server;
    if(server.initServer(8080) == false){
        return -1;
    }

    //创建epoll模型
    epfd = epoll_create(1);
    if(epfd == -1){
        perror("create epoll");
        return -1;
    }

    //创建事件，向epoll中添加需要检测的节点
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = server.m_listenfd;

    if(epoll_ctl(epfd, EPOLL_CTL_ADD, server.m_listenfd, &ev) == -1){
        perror("epoll_ctl");
    }

    struct epoll_event evs[1024];
    int evsize = sizeof(evs)/sizeof(struct epoll_event);

    while(1){
        int evnum = epoll_wait(epfd, evs, evsize, -1);
        for(int i = 0; i < evnum; i++){
            int curfd = evs[i].data.fd;
            if(curfd == server.m_listenfd){
                int clientfd = accept(curfd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = clientfd;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev) == -1){
                    perror("epoll_ctl");
                    break;
                }
            }
            else{
                TcpServer* client = new TcpServer;
                client->m_clientfd = curfd;
                thread th(doHTTPRequest, ref(client));
                th.join();
            }
        }
    }
    return 0;
}

void doHTTPRequest(void* arg){
    TcpServer* client = (TcpServer*)arg;
    string buf;
    string method;
    string url;
    string path;
    int len = getLine(client->m_clientfd, buf);
    if(len > 0){
        int i = 0;
        while(!isspace(buf[i])){
            method += buf[i++];
        }
        cout << "method: " << method << "\tThread ID: " << this_thread::get_id() << endl;

        if(method == "GET"){
            //获取url
            while(isspace(buf[i++]));
            while(!isspace(buf[i])){
                if(buf[i] == '?' || buf[i] == ':'){
                    break;
                }
                url += buf[i++];
            }
            cout << "url: " << url << endl;

            //获取请求打开文件路径
            path = "./html/" + url;

            //继续读取请求内容
            do{
                buf.clear();
                len = getLine(client->m_clientfd, buf);
            }while(len > 0);

            doHTTPResponse(client->m_clientfd, path);
        }
        else{
            do{
                buf.clear();
                len = getLine(client->m_clientfd, buf);
            }while(len > 0);
            unimplemented(client->m_clientfd);
        }
    }
    else{
        badRequest(client->m_clientfd);
    }
    delete client;
}

//读取一行
int getLine(const int& fd, string& buf){
    char ch = '\0';
    int count = 0;
    while(ch != '\n'){
        int len = recv(fd, &ch, 1, 0);
        if(len == 1){
            if(ch == '\r'){
                continue;
            }
            else if(ch == '\n'){
                break;
            }
            else{
                buf += ch;
                count++;
            }
        }
        else if(len == -1){
            perror("read failed");
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            count = -1;
            break;
        }
        else{
            cerr << "客户端已经关闭连接" << endl;
            epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
            close(fd);
            count = -1;
            break;
        }
    }
    return count;
}

void doHTTPResponse(const int& fd, const string path){
    ifstream resource;
    resource.open(path, ios::in);
    if(resource.is_open()){
        string head;
        head += "HTTP/1.1 200 OK\r\n";
        head += "Content-Type: text/html\r\n";
        head += "Connection: keep-alive\r\n\r\n";
        int len = send(fd, head.c_str(), head.size(), 0);
        if(len < 0) return;
        else{
            string buf;
            while(getline(resource, buf)){
                send(fd, buf.c_str(), buf.size(), 0);
            }
            resource.close();
        }
    }
    else{
        notFound(fd);
    }
}

