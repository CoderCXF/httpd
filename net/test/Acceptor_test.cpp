/*
 * @Author: your name
 * @Date: 2021-03-18 10:14:02
 * @LastEditTime: 2021-03-23 16:00:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/test/Acceptor_test.cpp
 */
#include "../Acceptor.h"
void usercallback(int connfd, AddrStruct peerAddr) {
    printf("new connection connected: peer IPort:%s\n How are you?",
            peerAddr.getIPort().c_str());
            sockets::close(connfd);
}
int main() {
    // 1. poller = epoll_create1()
    EventLoop loop;
    // 2. server sockaddr_in struct
    AddrStruct serv_addr(8989);
    // 3. socket -> bind ->  channel(channel && readcallback == handledRead) 
    Acceptor acceptor(&loop, serv_addr);
    acceptor.setNewConnectionCallback(usercallback);
    acceptor.listen();
    // epoll_wait()--->activeChannel-->channel::handleEvent -->handledRead-->accept
    loop.loop();
    return 0;
}