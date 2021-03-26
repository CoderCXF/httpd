/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:30
 * @LastEditTime: 2021-03-26 09:42:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Connection.h
 */
#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <functional>
#include "AddrStruct.h"
#include "Buffer.h"
#include "../base/Timestamp.h"
#include "../base/Logging.h"

// 前向声明
class EventLoop;
class Channel;
class Socket;

class Connection : public std::enable_shared_from_this<Connection>{
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&, int)> HighWaterMarkCallback;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;
    typedef std::function<void (const TcpConnectionPtr&,
                            Buffer*,
                            Timestamp)> MessageCallback;
    typedef std::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;


    // temporary
    // typedef std::function<void (const TcpConnectionPtr&,
    //                         const char*,
    //                         ssize_t len)> MessageCallback;
    Connection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const AddrStruct& localAddr,
                const AddrStruct& peerAddr);
    ~Connection();
    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
    void setMessasgeCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }
    void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
    void setHighWatermarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark) 
    { highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark; }
    
    //TODO:未完成
    /// TcpServer call
    void connectEstablished();
    void connectDestroy();
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();

    // 向客户端回送消息 & 服务器(主动)单向关闭shutdown
    void send(const char* data, int len);
    void send(const void *data, int len);
    void send(const std::string &data);
    void send(Buffer *data);

    void shutdown();
    // simple api
    const std::string name() { return connName; }
    const AddrStruct& localAddress() const { return localAddr_; }
    const AddrStruct& peerAddress() const { return peerAddr_; }
    void setTcpNoDelay(bool on);
    EventLoop* getLoop() { return loop_; }
    bool connected() const { return state_ == kconnected; }
private:
    enum StateE{kdisconnected, kconnecting, kconnected, kdisconnecting};
    void setState(StateE state) { state_ = state; }
    // send & shutdown
    void sendInLoop(const std::string &data);
    void sendInLoop(const char* buf, size_t len);
    void shutdownInLoop();
    
    EventLoop *loop_;
    const std::string connName;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    StateE state_;
    const AddrStruct localAddr_; //server sockaddr_in
    const AddrStruct peerAddr_;  // conn(client) sockaddr_in
    size_t highWaterMark_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_; // TcpServer::removeConnection
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;
};


#endif