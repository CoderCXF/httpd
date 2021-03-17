/*
 * @Author: your name
 * @Date: 2021-03-17 20:45:34
 * @LastEditTime: 2021-03-17 21:34:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Acceptor.h
 */
#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <functional>
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "AddrStruct.h"
#include "SocketOps.h"

class Acceptor{
public:
// typedef std::function<void(int sockfd, )>
    Acceptor(EventLoop *loop, AddrStruct serverAddr, int reusePort) 
        : loop_(loop),
          channel_(loop, loop->fd()),
          acceptScoket_(sockets::socketNoBlockOrDie()),
          listening_(false)
{
    
}


private:
    EventLoop *loop_;
    Channel channel_;
    Socket acceptScoket_;
    bool listening_;
};
#endif