/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:39
 * @LastEditTime: 2021-03-25 20:47:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/TcpServer.h
 */
#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "Acceptor.h"
#include "EventLoopThreadPool.h"
#include <map>
#include "Buffer.h"
#include "../base/Timestamp.h"

class EventLoop;
class Connection;
class TcpServer{
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;

    typedef std::function<void (const TcpConnectionPtr&,
                            Buffer*,
                            Timestamp)> MessageCallback;
    // typedef std::function<void (const TcpConnectionPtr&,
    //                         const char*,
    //                         ssize_t len)> MessageCallback;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
    typedef std::function<void(const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;

    TcpServer(EventLoop *loop, 
            AddrStruct &listenAddr,
            const std::string& name,
            bool portReused = true);
    ~TcpServer();

    void start();
    // 这是为用户提供的接口，用户调用该setConnectionCallback注册函数，
    // 然后会调用Connection::setConnectionCallback将该函数注册进Connection中
    // for user api
    void setConnectionCallback(const ConnectionCallback& cb) { connectioncallback_  = cb; }
    void setMessageCallback(const MessageCallback& cb) { messagecallback_  = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_  = cb; }
    void setThreadNum(int threadNum);

    // for debug
    int getThreadNums() { return threadpool_->getThreadNums(); }
private:
    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
    void newConnectionCallback(int sockfd, const AddrStruct& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);
    
    EventLoop *loop_;
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    bool started_;
    int nextConnId_;
    std::unique_ptr<EventLoopThreadPool> threadpool_;
    ConnectionCallback connectioncallback_;
    MessageCallback messagecallback_;
    CloseCallback closeCallback_; // == TcpServer::removeConnection
    WriteCompleteCallback writeCompleteCallback_;
    ConnectionMap connections_; // 维护一个连接链表
};
#endif