/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:41
 * @LastEditTime: 2021-03-23 16:40:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Connection.cpp
 */
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"
#include "Channel.h"
#include "SocketOps.h"

Connection::Connection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const AddrStruct& localAddr,
                const AddrStruct& peerAddr)
        :loop_(loop),
         connName(name),
         socket_(new Socket(sockfd)),
         channel_(new Channel(loop_, sockfd)),
         state_(kconnecting),
         localAddr_(localAddr),
         peerAddr_(peerAddr)
{
        // 通道可读事件到来的时候，会调用Connection::handleRead函数(也就是下面这个函数)
        channel_->setReadCallback(std::bind(&Connection::handleRead, this, std::placeholders::_1));
        sockets::setKeepAlive(true);
}
Connection::~Connection() 
{
        
}

void Connection::handleRead(Timestamp receiveTime)
{
//   loop_->assertInLoopThread();
//   int savedErrno = 0;
//   ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
//   if (n > 0)
//   {
//     messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
//   }
//   else if (n == 0)
//   {
//     handleClose();
//   }
//   else
//   {
//     errno = savedErrno;
//     LOG_SYSERR << "TcpConnection::handleRead";
//     handleError();
//   }
        loop_->assertInLoopThread();
        char buf[BUFSIZ];
        size_t n = ::read(channel_->fd(), buf, sizeof buf);
        messageCallback_(shared_from_this(), buf, n);
}


void Connection::connectEstablished() {
        loop_->assertInLoopThread();
        assert(state_ == StateE::kconnecting);
        setState(StateE::kconnected);
        channel_->enableReading();
        connectionCallback_(shared_from_this());
}