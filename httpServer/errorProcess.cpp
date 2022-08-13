#include"errorProcess.h"



void notFound(const int& fd){
    string path = "./html/not_found.html";
    ifstream resource;
    resource.open(path, ios::in);
    if(resource.is_open()){
        string buf;
        while(getline(resource, buf)){
            send(fd, buf.c_str(), buf.size(), 0);
        }
        resource.close();
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
        string buf;
        while(getline(resource, buf)){
            send(fd, buf.c_str(), buf.size(), 0);
        }
        resource.close();
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
        string buf;
        while(getline(resource, buf)){
            send(fd, buf.c_str(), buf.size(), 0);
        }
        resource.close();
    }
    else{
        return;
    }
}