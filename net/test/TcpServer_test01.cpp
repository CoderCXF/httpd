/*
 * @Author: your name
 * @Date: 2021-03-23 16:11:07
 * @LastEditTime: 2021-03-23 17:11:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/test/TcpServer_test01.cpp
 */
#include "../TcpServer.h"
#include "../EventLoop.h"
#include "../Connection.h"
typedef std::shared_ptr<Connection> TcpConnectionPtr;
void onConnection(const TcpConnectionPtr& conn) {
    if (conn->connected()) {
        printf("onConnection: new connection [%s] from [%s]\n",
        conn->name().c_str(),
        conn->peerAddress().getIPort().c_str());
    } else {
        printf("onConnection: connection [%s] is down\n",
        conn->name().c_str());
    }
}

void onMessage(const TcpConnectionPtr& conn,
                const char *data,
                ssize_t len) {
    printf("onMessage: receive [%ld] bytes from connection [%s]\n",
            len,
            conn->name().c_str());
}

int main() {
    EventLoop loop;
    AddrStruct listenAddr(9999);
    TcpServer server(&loop, listenAddr, "TcpServer_test01");
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();
    loop.loop();
    return 0;
}