#include"errorProcess.h"



void notFound(const int& fd){
    string path = "./html/not_found.html";
    ifstream resource;
    resource.open(path, ios::in);
    if(resource.is_open()){
        string head;
        head += "HTTP/1.1 400 NOT FOUND\r\n";
        head += "Content-Type: text/html\r\n\r\n";
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
        return;
    }
}

void unimplemented(const int& fd){
    string path = "./html/unimplemented.html";
    ifstream resource;
    resource.open(path, ios::in);
    if(resource.is_open()){
        string head;
        head += "HTTP/1.1 501 Method Not Implemented\r\n";
        head += "Content-Type: text/html\r\n\r\n";
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
        return;
    }
}

void badRequest(const int& fd){
    string path = "./html/bad_request.html";
    ifstream resource;
    resource.open(path, ios::in);
    if(resource.is_open()){
        string head;
        head += "HTTP/1.1 400 Bad Request\r\n";
        head += "Content-Type: text/html\r\n\r\n";
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
        return;
    }
}
