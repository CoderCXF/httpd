/*
 * @Author: your name
 * @Date: 2021-03-26 21:17:08
 * @LastEditTime: 2021-03-27 11:15:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/HttpServer.h
 */
#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <functional>
#include "../TcpServer.h"
#include "../Connection.h"
#include "../AddrStruct.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
class HttpServer{
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
    typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;

    typedef std::function<void (const TcpConnectionPtr&,
                            Buffer*,
                            Timestamp)> MessageCallback;
    typedef std::function<void (const HttpRequest&, HttpResponse*)> HttpCallback;
    HttpServer(EventLoop* loop,
             AddrStruct& listenAddr,
             const std::string& name);

    void setHttpCallback(const HttpCallback& cb) {
        httpCallback_ = cb;
    }

    void setThreadNum(int numThreads) {
        server_.setThreadNum(numThreads);
    }

    void start() {
        LOG_WARN << "HttpServer[" << server_.name()
        << "] starts listening on " << server_.ipPort();
        server_.start();
    }
    
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn,
                    Buffer *inputBuffer,
                    Timestamp receiveTime);
    
    void onRequest(const TcpConnectionPtr& conn, 
                      const HttpRequest &req);
    void defaultHttpCallback(const HttpRequest&, HttpResponse* resp);
    TcpServer server_;
    HttpCallback httpCallback_;
};

#endif
