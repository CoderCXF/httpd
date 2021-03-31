/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:48
 * @LastEditTime: 2021-03-31 15:42:56
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
            const std::string& name)
        :loop_(loop),
        ipPort_(listenAddr.getIPort()),
        name_(name),
        acceptor_(new Acceptor(loop_, listenAddr)), // import
        started_(false),
        nextConnId_(1),
        threadpool_(new EventLoopThreadPool(loop_, name_))
{
    // handleEvent会调用handleRead, 然后这里注册的回调函数是newConnection
    acceptor_->setNewConnectionCallback(
    std::bind(&TcpServer::newConnectionCallback, this, std::placeholders::_1, std::placeholders::_2));
}
TcpServer::~TcpServer() 
{
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

    for (auto& item : connections_)
    {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        // TODO:
        conn->getLoop()->runInLoop(
          std::bind(&Connection::connectDestroy, conn));
    }
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
    // 新的连接不放在main Reactor(acceptor所在的IO线程)，
    // 而是轮叫放至其他的IO线程（sub Reactor）中
    EventLoop *ioLoop = threadpool_->getNextLoop();
    // create a new connection
    TcpConnectionPtr conn(new Connection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
    connections_[connName] = conn;
    // 有新的连接到来，服务器所发送的信息                                        
    conn->setConnectionCallback(connectioncallback_);
    // 有连接发送消息到来，服务器所发送的信息
    conn->setMessasgeCallback(messagecallback_);
    // 断开连接，服务器所发送的消息
    conn->setCloseCallback(
        std::bind(&TcpServer::removeConnection, this, std::placeholders::_1));
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    // FIXME:
    // conn->connectEstablished();
    ioLoop->runInLoop(std::bind(&Connection::connectEstablished, conn));
}
void TcpServer::setThreadNum(int threadNum) {
    assert(threadNum >= 0);
    threadpool_->setThreadNum(threadNum);
}

void TcpServer::start() {
    if (!started_) {
        started_ = true;
        threadpool_->start();
    }
    if (!acceptor_->listening()) {
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
    // FIXME: why if cancle comment, will error ocurr ???
    // loop_->assertInLoopThread();
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
    loop_->assertInLoopThread();
    // erase connection obj from list
    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop *ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&Connection::connectDestroy, conn));
}