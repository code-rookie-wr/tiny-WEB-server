#pragma once
#include <functional>
#include "Epoll.h"
#include "../TCPServer/TcpSocket.h"

using namespace std;
using Callback = function<void()>;

class Channel{
public:
    Channel(const int& fd);
    ~Channel();

    void setReadHandler(Callback readHandler);
    void setWriteHandler(Callback writeHandler);
    void setCloseHandler(Callback closeHandler);
    void setEvents(int events);
    void handleEvent();
    int getfd();
    int getevents();

private:
    int _fd;
    int _events;
    Callback _readHandler;
    Callback _writeHandler;
    Callback _closeHandler;
};