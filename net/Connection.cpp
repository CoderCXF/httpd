/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:41
 * @LastEditTime: 2021-03-24 10:06:17
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
        // 通道可读事件到来的时候，会调用Channel::handleEvent,
        // Connection::handleRead函数(也就是下面这个函数)
        channel_->setReadCallback(std::bind(&Connection::handleRead, this, std::placeholders::_1));
        channel_->setCloseCallback(std::bind(&Connection::handleClose, this));
        channel_->setErrorCallback(std::bind(&Connection::handleError, this));
        sockets::setKeepAlive(true);
}
Connection::~Connection() 
{
        
}

void Connection::handleRead(Timestamp receiveTime)
{
        loop_->assertInLoopThread();
        // int savedErrno = 0;
        // ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
        // if (n > 0)
        // {
        // messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
        // }
        // else if (n == 0)
        // {
        // handleClose();
        // }
        // else
        // {
        // errno = savedErrno;
        // LOG_SYSERR << "TcpConnection::handleRead";
        // handleError();
        // }
        char buf[65535];
        size_t n = ::read(channel_->fd(), buf, sizeof buf);
        // messageCallback_(shared_from_this(), buf, n);
        if (n > 0)
        {
                messageCallback_(shared_from_this(), buf, sizeof buf);
        }
        else if (n == 0)
        {
                handleClose();
        }
        else
        {
                handleError();
        }
}
//
/// 建立连接，并真正的回调用户注册的connectionCallback
//
void Connection::connectEstablished() {
        loop_->assertInLoopThread();
        assert(state_ == StateE::kconnecting);
        setState(StateE::kconnected);
        channel_->tie(shared_from_this());// shared_ptr + 1
        // 开始监听读事件update(epoll_ctl(EPOLL_CTL_ADD))
        channel_->enableReading();
        connectionCallback_(shared_from_this());
}
//
/// 关闭连接
//
void Connection::connectDestroy() {
        loop_->assertInLoopThread();
        if(state_ == StateE::kconnected) {
                setState(StateE::kdisconnected);
                channel_->disableAll();
                connectionCallback_(shared_from_this());
        }
        channel_->remove();
}
//
/// 关闭连接,并真正的调用TcpServer::removeConnection == closeCallback
//
void Connection::handleClose() {
        LOG_DEBUG << "Connection::handlwClose()";
        loop_->assertInLoopThread();
        channel_->disableAll();
        setState(StateE::kdisconnected);
        // closeCallback是一个内部函数，和connectionCallback以及messageCallback不一样，
        // 这里的closeCallback实际上就是调用了TcpServer::removeConnection
        closeCallback_(shared_from_this()); 
}
//
/// handle error
//

void Connection::handleError() {
        LOG_ERROR << "Tcp::Connection";
}
