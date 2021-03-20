/*
 * @Author: your name
 * @Date: 2021-03-20 10:54:39
 * @LastEditTime: 2021-03-20 17:59:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/TcpServer.h
 */
#include "Acceptor.h"

class EventLoop;
class Connection;
class TcpServer{
    TcpServer(EventLoop *loop, 
            AddrStruct &listenAddr,
            const std::string& name,
            bool portReused = false);
    ~TcpServer();
    // epoll_wait-->handleEvent-->accept-->handleRead
    //-->newConnctionCallback--> TcpServer::newConnectionCallback
    void newConnectionCallback(int sockfd, const AddrStruct& peerAddr);
    void start();
private:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    EventLoop *loop_;
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    bool started_;
    int nextConnId_;
    
};