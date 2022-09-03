#include "Channel.h"

Channel::Channel(const int& fd){
    _fd = fd;
}

Channel::~Channel(){
    if(_fd >= 0){
        close(_fd);
        _fd = -1;
    }
}

void Channel::setReadHandler(Callback readHandler){
    _readHandler = readHandler;
}

void Channel::setWriteHandler(Callback writeHandler){
    _writeHandler = writeHandler;
}

void Channel::setCloseHandler(Callback closeHandler){
    _closeHandler = closeHandler;
}

void Channel::setEvents(int events){
    _events = events;
}

void Channel::handleEvent(){
    if(_events & EPOLLIN){
        _readHandler();
    }
    else if(_events & EPOLLOUT){
        _writeHandler();
    }
}

int Channel::getfd(){
    return _fd;
}

int Channel::getevents(){
    return _events;
}