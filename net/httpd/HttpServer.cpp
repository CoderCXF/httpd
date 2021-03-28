/*
 * @Author: your name
 * @Date: 2021-03-26 21:17:17
 * @LastEditTime: 2021-03-28 10:25:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/HttpServer.cpp
 */
#include "HttpServer.h"
#include "HttpParse.h"
#include "HttpResponse.h"
// #include ""
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
HttpServer::HttpServer(EventLoop* loop,
             AddrStruct& listenAddr,
             const std::string& name)
        :server_(loop, listenAddr, name)
{
    server_.setConnectionCallback(
        std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
    server_.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2, _3));
}

void HttpServer::onConnection(const TcpConnectionPtr& conn) {
    // TODO: forbid Nagle
    conn->setTcpNoDelay(true);
    if (conn->connected()) {
        LOG_INFO << "new connection from [ipPort = " << conn->peerAddress().getIPort().c_str() << "] is up";
    } else {
        LOG_INFO << "conection  [ipPort =" << conn->peerAddress().getIPort().c_str() << "] is down";
    }
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
                            Buffer *inputBuffer,
                            Timestamp receiveTime) 
{
    // TODO: comment LOG
    // printf("new message visist\n");
    bool succeed = true;
    // FIXME:
    std::shared_ptr<HttpParse> parse(new HttpParse());
    // HttpParse *parse = new HttpParse();
    succeed = parse->parseRequest(inputBuffer);
    // 可以通过request接口获取解析的请求
    if (!succeed) {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
        conn->shutdown();
    }
    if (parse->isComplete()) {
        onRequest(conn, parse->request());
        // FIXME: is OK?
        parse->reset();
    }
}

void HttpServer::onRequest(const TcpConnectionPtr& conn, 
                           const HttpRequest &request) 
{
    HttpResponse response;
    std::string closeStr = request.getHead("Connection");
    if (closeStr == "close" || closeStr != "Keep-Alive" 
        || request.version() == HttpRequest::kHttp10) 
    {
        response.setVersion(HttpResponse::kHttp10);
    } else {
        response.setVersion(HttpResponse::kHttp11);
    } 
    // 由用户(回调函数)填充响应内容
    httpCallback_(request, &response);
    Buffer buf;
    response.appendBuf(&buf);                   
    conn->send(&buf);
    if (response.version() == HttpResponse::kHttp10) {
        conn->shutdown();
    }
}

void HttpServer::defaultHttpCallback(const HttpRequest&, HttpResponse* resp)
{
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    // resp->setCloseConnection(true);
}
