/*
 * @Author: your name
 * @Date: 2021-03-17 09:42:28
 * @LastEditTime: 2021-03-27 21:26:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Editna
 * @FilePath: /WebServer/net/SocketOps.cpp
 */

#include <sys/types.h>
#include <sys/socket.h>
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
// socket func
///
int sockets::socketNoBlockOrDie() {
    int lfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (lfd < 0) {
        LOG_SYSFATAL << "sockets::sockeNoBlockOrDie";
    }
    return lfd;
}

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

