/*
 * @Author: your name
 * @Date: 2021-03-20 14:34:30
 * @LastEditTime: 2021-03-23 20:34:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Connection.h
 */
#ifndef CONNECTION_H
#define CONNECTION_H

#include "AddrStruct.h"
#include "../base/Timestamp.h"
#include "../base/Logging.h"
#include <memory>

// 前向声明
class EventLoop;
class Channel;
class Socket;

class Connection : public std::enable_shared_from_this<Connection>{
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;;
    // typedef std::function<void (const TcpConnectionPtr&,
    //                         Buffer*,
    //                         Timestamp)> MessageCallback;
    // temporary
    typedef std::function<void (const TcpConnectionPtr&,
                            const char*,
                            ssize_t len)> MessageCallback;
    Connection(EventLoop* loop,
                const std::string& name,
                int sockfd,
                const AddrStruct& localAddr,
                const AddrStruct& peerAddr);
    ~Connection();
    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
    void setMessasgeCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void setCloseCallback(const CloseCallback& cb) { closeCallback_ = cb; }
    
    //TODO:未完成
    /// TcpServer call
    void connectEstablished();
    void connectDestroy();
    void handleRead(Timestamp receiveTime);
    void handleClose();
    void handleError();
    // simple api
    const std::string name() { return connName; }
    const AddrStruct& localAddress() const { return localAddr_; }
    const AddrStruct& peerAddress() const { return peerAddr_; }
    EventLoop* getLoop() { return loop_; }
    bool connected() const { return state_ == kconnected; }
private:
    enum StateE{kdisconnected, kconnecting, kconnected, kdisconnecting};
    void setState(StateE state) { state_ = state; }
    EventLoop *loop_;
    const std::string connName;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    StateE state_;
    const AddrStruct localAddr_; //server sockaddr_in
    const AddrStruct peerAddr_;  // conn(client) sockaddr_in
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_; // TcpServer::removeConnection
};


#endif