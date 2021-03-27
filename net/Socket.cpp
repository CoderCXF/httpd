/*
 * @Author: your name
 * @Date: 2021-03-17 10:18:07
 * @LastEditTime: 2021-03-27 21:29:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Socket.cpp
 */
#include <fcntl.h>
#include <netinet/tcp.h> //TCP_NODELAY
#include "Socket.h"
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

///
//options func
///
void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}

void Socket::setKeepAlive(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
}

int Socket::setSocketNonBlocking() {
    int flags = ::fcntl(sockfd_, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = 0;
    if ((ret = fcntl(sockfd_, F_SETFL, flags)) == -1) {
        LOG_SYSFATAL << "sockets::setSocketNonBlocking";
    }
    return ret;
}