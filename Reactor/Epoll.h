#pragma once

#include <iostream>
#include <sys/epoll.h>
#include <vector>
#include <memory>
#include "Channel.h"
#include <unordered_map>

using namespace std;

const int EVENTSIZE = 1024;

class Channel;
class Epoll{
public:
    Epoll();
    ~Epoll();

    void epollAdd(const shared_ptr<Channel>& channel);
    void epollDel(const shared_ptr<Channel>& channel);
    void epollMod(const shared_ptr<Channel>& channel);
    int epollWait(vector<shared_ptr<Channel>>& vecChannel);

private:
    int _epfd;
    vector<struct epoll_event> _evs;
    unordered_map<int, shared_ptr<Channel>> _channelmap;
};