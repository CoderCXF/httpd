/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:41
 * @LastEditTime: 2021-03-28 11:26:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Connection.cpp
 */
#include "Connection.h"
#include "EventLoop.h"
#include "Channel.h"
#include "Socket.h"
#include "SocketOps.h"

Connection::Connection(EventLoop *loop,
                       const std::string &name,
                       int sockfd,
                       const AddrStruct &localAddr,
                       const AddrStruct &peerAddr)
    : loop_(loop),
      connName(name),
      socket_(new Socket(sockfd)),
      channel_(new Channel(loop_, sockfd)),
      state_(kconnecting),
      localAddr_(localAddr),
      peerAddr_(peerAddr),
      highWaterMark_(64 * 1024 * 1024)
{
        // 通道可读事件到来的时候，会调用Channel::handleEvent,
        // Connection::handleRead函数(也就是下面这个函数)
        channel_->setReadCallback(std::bind(&Connection::handleRead, this, std::placeholders::_1));
        channel_->setWriteCallback(std::bind(&Connection::handleWrite, this));
        channel_->setCloseCallback(std::bind(&Connection::handleClose, this));
        channel_->setErrorCallback(std::bind(&Connection::handleError, this));
        // TODO:
        // 只要是设置了keep-alive，就表明服务器支持长连接了.
        // 但是客户端也必须发来keep-alive才能最后真正实现长连接，并不是说服务器这端设置了就是长连接了。
        socket_->setKeepAlive(true); 
}
Connection::~Connection()
{
}

void Connection::handleRead(Timestamp receiveTime)
{
        loop_->assertInLoopThread();
        int saveErrno = 0;
        ssize_t n = inputBuffer_.readFd(channel_->fd(), &saveErrno);
        // messageCallback_(shared_from_this(), buf, n);
        if (n > 0)
        {
                // messageCallback_(shared_from_this(), buf, sizeof buf);
                messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
        }
        else if (n == 0)
        {
                // TODO: comment LOG
                // LOG_DEBUG << "Cononection::handleRead";
                handleClose();
        }
        else
        {
                handleError();
        }
}

// 处理写事件，即负责将应用层buffer中的内容全部写入内核缓冲区
void Connection::handleWrite()
{
        // TODO:
        loop_->assertInLoopThread();
        if (channel_->isWriting())
        {
                ssize_t n = sockets::write(channel_->fd(),
                                           outputBuffer_.peek(),
                                           outputBuffer_.readableBytes());
                if (n > 0)
                {
                        outputBuffer_.retrieve(n);
                        // 一旦缓冲区已经全部写入内核缓冲区,停止监听写事件
                        if (outputBuffer_.readableBytes() == 0)
                        {
                                // 一旦缓冲区已经全部写入内核缓冲区,停止监听写事件
                                channel_->disableWriting();
                                if (writeCompleteCallback_)
                                {
                                        loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                                }
                                if (state_ == kdisconnecting)
                                {
                                        shutdownInLoop();
                                }
                        }
                }
                else
                {

                        LOG_SYSERR << "TcpConnection::handleWrite";
                        // if (state_ == kDisconnecting)
                        // {
                        //   shutdownInLoop();
                        // }
                }
        }
        else
        {
                // TODO: comment LOG
                // LOG_TRACE << "Connection fd = " << channel_->fd()
                //           << " is down, no more writing";
        }
}
//
/// 建立连接，并真正的回调用户注册的connectionCallback
//
void Connection::connectEstablished()
{
        loop_->assertInLoopThread();
        assert(state_ == StateE::kconnecting);
        setState(StateE::kconnected);
        channel_->tie(shared_from_this()); // shared_ptr + 1
        /// 监听用户的读事件并且回调用户的函数
        // 开始监听读事件update(epoll_ctl(EPOLL_CTL_ADD))
        channel_->enableReading();
        // 执行用户回调的onConnection中的else语句
        connectionCallback_(shared_from_this());
}
//
/// 关闭连接
//
void Connection::connectDestroy()
{
        loop_->assertInLoopThread();
        if (state_ == StateE::kconnected)
        {
                setState(StateE::kdisconnected);
                channel_->disableAll();
                connectionCallback_(shared_from_this());
        }
        channel_->remove();
}
//
/// 关闭连接,并真正的调用TcpServer::removeConnection == closeCallback
//
void Connection::handleClose()
{
        loop_->assertInLoopThread();
        setState(StateE::kdisconnected);
        channel_->disableAll();
        // 执行用户回调的onConnection中的else语句
        connectionCallback_(shared_from_this());
        // closeCallback是一个内部函数，和connectionCallback以及messageCallback不一样，
        // 这里的closeCallback实际上就是调用了TcpServer::removeConnection
        closeCallback_(shared_from_this());
}
//
/// handle error
//

void Connection::handleError()
{
        LOG_ERROR << "Tcp::Connection";
}
//
/// 由于有函数重载，所以需要显示绑定（指定函数返回值和形参列表）
//
void Connection::send(const char *data, int len)
{
        if (state_ == StateE::kconnected && loop_->isInLoopThread())
        {
                sendInLoop(std::forward<std::string>(data));
        }
        else
        {
                loop_->runInLoop(std::bind(
                    (void (Connection::*)(const std::string &)) & Connection::sendInLoop,
                    this,
                    std::forward<std::string>(data)));
        }
}

void Connection::send(const void *data, int len)
{
        send(static_cast<const char *>(data), len);
}

void Connection::send(const std::string &data)
{
        if (state_ == StateE::kconnected && loop_->isInLoopThread())
        {
                sendInLoop(data);
        }
        else
        {
                loop_->runInLoop(std::bind(
                    (void (Connection::*)(const std::string &)) & Connection::sendInLoop,
                    this,
                    data));
        }
}
void Connection::send(Buffer *data)
{
        if (state_ == StateE::kconnected && loop_->isInLoopThread())
        {
                sendInLoop(data->peek(), data->readableBytes());
                data->retrieveAll();
        }
        else
        {
                loop_->runInLoop(std::bind(
                    (void (Connection::*)(const std::string &)) & Connection::sendInLoop,
                    this,
                    data->retrieveAllAsString()));
        }
}

// 到目前为止，我们只是关注了已连接socket的写事件，但是什么时候关注读事件呢？？？
// 其实也很简单，只要套接字的发送缓冲区有数据的时候，就认为要有EPOLL_OUT事件产生。
void Connection::sendInLoop(const std::string &data)
{
        assert(loop_->isInLoopThread());
        // sockets::write(channel_->fd(), data.c_str(), data.size());
        sendInLoop(data.c_str(), data.size());
}

void Connection::sendInLoop(const char *buf, size_t len)
{
        size_t remaining = len;
        ssize_t nwrote = 0;
        if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
        {
                //这个时候应用层缓冲区没有数据，所以直接write即可
                nwrote = sockets::write(channel_->fd(), buf, len);
                if (nwrote >= 0)
                {
                        remaining = len - nwrote;
                        if (remaining == 0 && writeCompleteCallback_)
                        {
                                loop_->queueInLoop(std::bind(&Connection::writeCompleteCallback_, shared_from_this()));
                        }
                }
                else
                {
                        // handle error
                        nwrote = 0;
                        LOG_SYSERR << "Connection::sendInLoop";
                }
        }

        assert(remaining <= len);
        // 如果一次性没有写完，意味着发送内核缓冲区已满，所以需要写入应用层buffer(output buffer)
        if (remaining > 0)
        {
                size_t oldLen = outputBuffer_.readableBytes();
                if (oldLen + remaining >= highWaterMark_ && oldLen < highWaterMark_ && highWaterMarkCallback_)
                {
                        loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
                }
                outputBuffer_.append(static_cast<const char *>(buf) + nwrote, remaining);
                if (!channel_->isWriting())
                {
                        // 开始监听写事件（服务器-->客户端）
                        // 只要是server调用了send函数就开始监听写事件
                        // （enableWriting-->Channel::update-->EPoll::updateChannel(epoll_ctl(EPOLL_CTL_ADD, EPOLL_OUT))）
                        channel_->enableWriting();
                }
        }
}
// 关闭写端，必须等到缓冲区数据全部写完之后再关闭
void Connection::shutdown()
{
        // FIXME: use compare and swap
        if (state_ == kconnected)
        {
                setState(kdisconnecting);
                // FIXME: shared_from_this()?
                loop_->runInLoop(std::bind(&Connection::shutdownInLoop, shared_from_this()));
        }
}

void Connection::shutdownInLoop()
{
        loop_->assertInLoopThread();
        // 如果没有关注读事件的话
        if (!channel_->isWriting())
        {
                // we are not writing
                sockets::shutdownRDWR(channel_->fd());
        }
}

void Connection::setTcpNoDelay(bool on) 
{ 
        socket_->setTcpNoDelay(on); 
}

void Connection::setKeepAlive(bool on) {
        socket_->setKeepAlive(on);
}
