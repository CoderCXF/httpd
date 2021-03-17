/*
 * @Author: your name
 * @Date: 2021-03-17 09:42:28
 * @LastEditTime: 2021-03-17 19:36:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Editna
 * @FilePath: /WebServer/net/SocketOps.cpp
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h> //TCP_NODELAY
#include <fcntl.h>
#include <unistd.h>
#include"../base/Logging.h"
#include "SocketOps.h"

///
// base read/write func
///
ssize_t sockets::read(int sockfd, void *buf, size_t count) {
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt) {
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count) {
    return ::write(sockfd, buf, count);
}

void sockets::close(int sockfd) {
    int ret = ::close(sockfd);
    if (ret < 0) {
        LOG_SYSFATAL << "sockets::close";
    }
}
///
// shutdown func
///
void    sockets::shutdownRead(int sockfd) {
    ::shutdown(sockfd, SHUT_RD);
}

void    sockets::shutdownWrite(int sockfd) {
    ::shutdown(sockfd, SHUT_WR);
}

void    sockets::shutdownRDWR(int sockfd) {
    ::shutdown(sockfd, SHUT_RDWR);
}
///
//options func
///
void    sockets::setReuseAddr(int sockfd, bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}

void    sockets::setTcpNoDelay(int sockfd, bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
}

void sockets::setKeepAlive(int sockfd, bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
}

int     sockets::setSocketNonBlocking(int sockfd) {
    int flags = ::fcntl(sockfd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = 0;
    if ((ret = fcntl(sockfd, F_SETFL, flags)) == -1) {
        LOG_SYSFATAL << "sockets::setSocketNonBlocking";
    }
    return ret;
}


///
// socket func
///
int sockets::bindOrDie(int sockfd, const struct sockaddr *addr) {
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr)));
    if (ret < 0) {
        LOG_SYSFATAL << "socket::bindOrDie";
    }
    return ret;
}

int sockets::listenOrDie(int sockfd) {
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0) {
        LOG_SYSFATAL << "socket::listenOrDie";
    }
    return ret;
}

int  sockets::accept(int sockfd, struct sockaddr* addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof(struct sockaddr));
    int connfd = ::accept(sockfd, addr, &addrlen);
    if (connfd < 0) {
        LOG_SYSFATAL << "sockets::accept";
        return -1;
    }
    return connfd;
}


int sockets::connect(int sockfd, const struct sockaddr* addr)
{
    // No error
  return ::connect(sockfd, addr, static_cast<socklen_t>(sizeof(struct sockaddr_in6)));
}

