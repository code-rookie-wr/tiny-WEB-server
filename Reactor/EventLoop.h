#pragma once
#include <memory>
#include <unordered_map>
#include <atomic>
#include <sys/eventfd.h>
#include "Epoll.h"

using namespace std;

class Channel;
class Epoll;

class EventLoop{
public:
    EventLoop();
    ~EventLoop();

    void loop(const int& threadNum = 1);
    void pollerAdd(shared_ptr<Channel> channel);
    void pollerDel(shared_ptr<Channel> channel);
    void pollerMod(shared_ptr<Channel> channel);

private:
    atomic<bool> _looping;
    shared_ptr<Epoll> _poller;
};