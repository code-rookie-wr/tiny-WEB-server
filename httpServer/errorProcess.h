#ifndef ERRORPROCESS_H
#define ERRORPROCESS_H

#include "../TCPServer/TcpSocket.h"
#include<string>
#include<fstream>
using namespace std;

void notFound(const int& fd);
void unimplemented(const int& fd);
void badRequest(const int& fd);

#endif