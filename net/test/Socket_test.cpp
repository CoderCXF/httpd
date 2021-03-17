/*
 * @Author: your name
 * @Date: 2021-03-17 14:57:23
 * @LastEditTime: 2021-03-17 20:22:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/test/Socket_test.cpp
 */
#include <stdio.h>
#include "../Socket.h"
#include "../AddrStruct.h"
// #include ""

int main() {
    
    int lfd = ::socket(AF_INET, SOCK_STREAM, 0);
    Socket socket(lfd);
    AddrStruct myaddr(9876);
    AddrStruct peerAddr;
    printf("my ip is: %s\n", myaddr.getIP().c_str());
    printf("my port is: %u\n", myaddr.getPort());
    printf("my ip:port is %s\n", myaddr.getIPort().c_str());
    // Server Wrapped
    // Acceptor class
    socket.Bind(myaddr);
    socket.Listen();
    int cfd = socket.Accept(peerAddr);
    if (cfd < 0) {
        printf("no connection\n");
    }
    printf("peer ip is: %s\n", peerAddr.getIP().c_str());
    printf("peer port is: %u\n", peerAddr.getPort());
    printf("peer ip:port is %s\n", peerAddr.getIPort().c_str());
    return 0;
}