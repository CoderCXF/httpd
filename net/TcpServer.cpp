/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:48
 * @LastEditTime: 2021-03-21 08:26:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/TcpServer.cpp
 */
#include "TcpServer.h"
#include "Connection.h"
#include "../base/Logging.h"
#include <functional> // for std::bind
TcpServer::TcpServer(EventLoop *loop, 
            AddrStruct &listenAddr,
            const std::string& name,
            bool portReused = false)
            :loop_(loop),
            ipPort_(listenAddr.getIPort()),
            name_(name),
            acceptor_(new Acceptor(loop_, listenAddr)),
            started_(false),
            nextConnId_(1)
{
    // handleEvent会调用handleRead, 然后这里注册的回调函数是newConnection
    acceptor_->setNewConnectionCallback(
    std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1, std::placeholders::_2));
}

//新建一个Connection 对象，并设置Connection的回调函数
void TcpServer::newConnectionCallback(int sockfd, const AddrStruct& peerAddr) {
    loop_->assertInLoopThread();
    char buf[64];
    snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;
    struct sockaddr_in localAddrIn;
    memset(&localAddrIn, 0, sizeof localAddrIn);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localAddrIn);
    if (::getsockname(sockfd, (struct sockaddr*)(&localAddrIn), &addrlen) < 0) {
        LOG_SYSERR << "sockets::getLocalAddr";
    }
    // localAddr = server addr   
    AddrStruct localAddr(localAddrIn);
    // create a new connection
    TcpConnectionPtr conn(new Connection(loop_,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
    connections_[connName] = conn;                                        
    conn->setConnectionCallback(connectioncallback_);
    conn->setMessasgeCallback(messagecallback_);
    conn->connectEstablished();
}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
    }
    if (!acceptor_->listening()) {
        loop_->runInloop(std::bind(&Acceptor::listen(), acceptor_.get()));
    }
}
