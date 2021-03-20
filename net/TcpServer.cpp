/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:48
 * @LastEditTime: 2021-03-20 21:47:37
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
    // Channel::handleEvent-->Acceptor-->handleRead-->Channel::newConnectionCallback == Tcp::server::newConnectionCallback-->
    acceptor_->setNewConnectionCallback(
    std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1, std::placeholders::_2));
}

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
    AddrStruct localAddr(localAddrIn);
    // TcpConnection class 
    TcpConnectionPtr conn(new Connection(Loop_,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
    }
    if (!acceptor_->listening()) {
        loop_->runInloop(std::bind(&Acceptor::listen(), acceptor_.get()));
    }
}
