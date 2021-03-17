/*
 * @Author: your name
 * @Date: 2021-03-17 10:18:07
 * @LastEditTime: 2021-03-17 19:53:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Socket.cpp
 */
#include "Socket.h"
#include <sys/socket.h>
#include "../base/Logging.h"
#include "SocketOps.h"

// RAII
Socket::~Socket() {
    sockets::close(sockfd_);
}

void Socket::Bind(const AddrStruct& addr) {
    sockets::bindOrDie(sockfd_, addr.getSocketAddr());
}

void Socket::Listen() {
    sockets::listenOrDie(sockfd_);
}

int Socket::Accept(AddrStruct& peeraddr) {
    //peeraddr ouput paramter
    // client sockaddr struct
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    int connfd = sockets::accept(sockfd_, (struct sockaddr *)(&addr));
    if (connfd >= 0)
    {
        //AddrStruct::setSockAddrInet
        peeraddr.setSockAddrInet(addr);
    }
    return connfd;
}