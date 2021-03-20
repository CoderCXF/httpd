/*
 * @Author: your name
 * @Date: 2021-03-17 20:45:34
 * @LastEditTime: 2021-03-20 15:00:15
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
    typedef std::function<void(int sockfd, AddrStruct addr)> NewConnectionCallback;
    Acceptor(EventLoop *loop, AddrStruct serverAddr, int reusePort = 1);
    ~Acceptor();
    void handleRead();
    void listen();
    bool listening() { return listening_; }
    void setNewConnectionCallback(NewConnectionCallback cb) {
        newConnectionCallback_ = cb;
    }
private:
    EventLoop *loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;;
    bool listening_;
};
#endif