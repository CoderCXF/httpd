/*
 * @Author: your name
 * @Date: 2021-03-28 17:03:16
 * @LastEditTime: 2021-03-28 17:45:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /httpd/net/test/TcpServer_test_IGN_SIGPIPIE.cpp
 */
#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../Connection.h"
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Server{
public:
    typedef std::shared_ptr<Connection> TcpConnectionPtr;
    Server(EventLoop *loop, 
              AddrStruct& listenAddr, 
              const std::string &servername)
            : loop_(loop),
              server_(loop_, listenAddr, servername)
    {
        server_.setConnectionCallback(std::bind(&Server::onConnection, this, _1));
        server_.setMessageCallback(std::bind(&Server::onMessage, this, _1, _2, _3));

    }
    void onConnection(const TcpConnectionPtr& conn) {
        if (conn->connected()) {
            printf("onConnection::newConnection established from [ip:port = %s name = %s] \n",
               conn->peerAddress().getIPort().c_str(),
               conn->name().c_str());
               // TODO:
            printf("send two message to peer\n");
            sleep(5);
            conn->send("hello world1");
            conn->send("hello world2");
            
        } else {
            printf("onConnection::connection [ip:port = %s name = %s] is down\n",
                conn->peerAddress().getIPort().c_str(),
                conn->name().c_str());
        }
        
    }

    void onMessage(const TcpConnectionPtr& conn,
                    Buffer *inputbuffer,
                    Timestamp receiveTime) {
        std::string msg(inputbuffer->retrieveAllAsString());
        printf("receive message [%s] from [ip:port = %s name = %s]\n",
                msg.c_str(),
                conn->peerAddress().getIPort().c_str(),
                conn->name().c_str());
    }

    void start() {
        server_.start();
    }

    // no used
    void setThreadNum(int nums) {
        server_.setThreadNum(nums);
    }

private:
    EventLoop *loop_;
    TcpServer server_;
};


int main() {
    EventLoop loop;
    AddrStruct listenAddr(9999);
    Server server(&loop, listenAddr, "SIGPIPE_TEST_SERVER");
    server.start();
    loop.loop();
    return 0;
}