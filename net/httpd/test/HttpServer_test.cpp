/*
 * @Author: your name
 * @Date: 2021-03-27 09:45:53
 * @LastEditTime: 2021-03-28 15:27:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/httpd/test/HttpServer_test.cpp
 */
#include "../HttpServer.h"
#include "../HttpRequest.h"
#include "../HttpResponse.h"
#include "../../EventLoop.h"
#include "../../../base/Logging.h"

#include <iostream>
#include <map>

extern char favicon[555];
bool benchmark = false;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
//   std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<std::string, std::string>& headers = req.headers();
    for (const auto& header : headers)
    {
      std::cout << header.first << ": " << header.second << std::endl;
    }
  }

  if (req.path() == "/")
  {
    resp->setStatusCode(HttpResponse::k200OK);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "Muduo");
    std::string now = Timestamp::now().toFormattedString();
    // resp->setBody("<html><head><title>Love</title></head>"
    //     "<body><h1>Hello</h1>Now is " + now +
    //     "</body></html>");
    resp->setBody("<html><head><title>Love</title></head>"
                  "<body><div style=\"margin: 0 auto; left: 50%; right: 50%;\">"
                  "<span style = \"color: red; font-size: 3em;\">Love"
                  "</span></div></body></html>");
  }
  else if (req.path() == "/hello")
  {
    resp->setStatusCode(HttpResponse::k200OK);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "httpd");
    resp->setBody("hello, world!\n");
  }
  else
  {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    // resp->setCloseConnection(true);
  }
}

int main(int argc, char* argv[])
{
  int numThreads = 0;
  if (argc <= 1) {
    LOG_WARN << "[defalut one loop started]";
  }
  if (argc > 1)
  {
    printf("argc = %d\n", argc);
    benchmark = true;
    // Logger::setLogLevel(Logger::WARN);
    LOG_INFO << "[" << argv[1] << " loops has started!]";
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  AddrStruct listenAddr(9999);
  HttpServer server(&loop, listenAddr, "httpd");

  server.setHttpCallback(onRequest);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
}