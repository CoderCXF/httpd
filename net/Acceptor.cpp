/*
 * @Author: your name
 * @Date: 2021-03-17 20:45:47
 * @LastEditTime: 2021-03-20 15:00:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Acceptor.cpp
 */
#include "Acceptor.h"
#include "../base/Logging.h"

Acceptor::Acceptor(EventLoop *loop, AddrStruct serverAddr, int reusePort) 
        : loop_(loop),
          acceptSocket_(sockets::socketNoBlockOrDie()),
          acceptChannel_(loop, acceptSocket_.fd()),
          listening_(false)
{
    sockets::setReuseAddr(reusePort);
    acceptSocket_.Bind(serverAddr);
    // post callback(handleEvent use this callback)
    acceptChannel_.setReadCallback(
      std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listening_ = true;
    acceptSocket_.Listen();
    acceptChannel_.enableReading();  //update-->epoll_ctl(EPOLL_ACTL_ADD, )
}

void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    AddrStruct peerAddr;
    //FIXME loop until no more
    int connfd = acceptSocket_.Accept(peerAddr);
    if (connfd >= 0)
    {
        if (newConnectionCallback_)
        {
        newConnectionCallback_(connfd, peerAddr);
        }
        else
        {
        sockets::close(connfd);
        }
    }
    else
    {
        LOG_SYSERR << "in Acceptor::handleRead";
        // Read the section named "The special problem of
        // accept()ing when you can't" in libev's doc.
        // By Marc Lehmann, author of libev.
        // if (errno == EMFILE)
        // {
        // ::close(idleFd_);
        // idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
        // ::close(idleFd_);
        // idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        // }
    }
}
