/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:39
 * @LastEditTime: 2021-03-23 15:04:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/TcpServer.h
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "Acceptor.h"
#include <map>

class EventLoop;
class Connection;
class TcpServer{
    
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    // typedef std::function<void (const TcpConnectionPtr&,
    //                         Buffer*,
    //                         Timestamp)> MessageCallback;
    typedef std::function<void (const TcpConnectionPtr&,
                            const char*,
                            ssize_t len)> MessageCallback;
    TcpServer(EventLoop *loop, 
            AddrStruct &listenAddr,
            const std::string& name,
            bool portReused = false);
    ~TcpServer();
    // epoll_wait-->handleEvent-->accept-->handleRead
    //-->newConnctionCallback--> TcpServer::newConnectionCallback
    void start();
    // 这是为用户提供的接口，用户调用该setConnectionCallback注册函数，
    // 然后会调用Connection::setConnectionCallback将该函数注册进Connection中
    void setConnectionCallback(const ConnectionCallback& cb) { connectioncallback_  = cb; }
    void setMessageCallback(const MessageCallback& cb) { messagecallback_  = cb; }
private:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
    void newConnectionCallback(int sockfd, const AddrStruct& peerAddr);
    EventLoop *loop_;
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    bool started_;
    int nextConnId_;
    ConnectionCallback connectioncallback_;
    MessageCallback messagecallback_;
    ConnectionMap connections_; // 维护一个连接链表
};
#endif