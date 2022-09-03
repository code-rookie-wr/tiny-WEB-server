#include "Epoll.h"
#include <string>

Epoll::Epoll(){
    _epfd = epoll_create(1024);
    _evs.resize(EVENTSIZE);
}

Epoll::~Epoll(){
    if(_epfd >= 0){
        close(_epfd);
    }
    _epfd = -1;
}

void Epoll::epollAdd(const shared_ptr<Channel>& channel){
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    int fd = channel->getfd();
    ev.data.fd = fd;
    epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
    _channelmap[fd] = move(channel);
}

void Epoll::epollDel(const shared_ptr<Channel>& channel){
    struct epoll_event ev;
    ev.events = EPOLLOUT;
    int fd = channel->getfd();
    ev.data.fd = fd;
    epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev);
    _channelmap.erase(fd);
}

void Epoll::epollMod(const shared_ptr<Channel>& channel){
    struct epoll_event ev;
    ev.events = channel->getevents();
    int fd = channel->getfd();
    ev.data.fd = fd;
    epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev);
}

int Epoll::epollWait(vector<shared_ptr<Channel>>& vecChannel){
    int nums = epoll_wait(_epfd, &*_evs.begin(), EVENTSIZE, -1);
    for(int i = 0; i < nums; ++i){
        int fd = _evs[i].data.fd;
        shared_ptr<Channel> temp = _channelmap[fd];
        temp->setEvents(_evs[i].events);
        vecChannel.emplace_back(move(temp));
    }
    return nums;
}