#include "EventLoop.h"
#include "../ThreadPool/ThreadPool.h"

EventLoop::EventLoop(){
    _looping = false;
    _poller = make_shared<Epoll>();
}

EventLoop::~EventLoop() {
    _looping = false;
}

void EventLoop::pollerAdd(shared_ptr<Channel> channel){
    _poller->epollAdd(channel);
}

void EventLoop::pollerDel(shared_ptr<Channel> channel){
    _poller->epollDel(channel);
}

void EventLoop::pollerMod(shared_ptr<Channel> channel){
    _poller->epollMod(channel);
}

void EventLoop::loop(const int& threadNum){
    _looping = true;
    while(_looping){
        vector<shared_ptr<Channel>> vecChannel;
        int nums = _poller->epollWait(vecChannel);
        ThreadPool* pool = new ThreadPool(threadNum);
        for(auto& channel : vecChannel){
            pool->addTask([&](){
                channel->handleEvent();
            });
        }
        vecChannel.clear();
        delete pool;
    }
}