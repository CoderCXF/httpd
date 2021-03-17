/*
 * @Author: your name
 * @Date: 2021-03-17 09:42:28
 * @LastEditTime: 2021-03-17 11:48:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Editna
 * @FilePath: /WebServer/net/SocketOps.cpp
 */

#include <sys/types.h>
#include <sys/socket.h>
#include"../base/Logging.h"
#include "SocketOps.h"

///
// base read/write func
ssize_t sockets::read(int sockfd, void *buf, size_t count) {
    return ::read(sockfd, buf, count);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt) {
    return ::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count) {
    return ::write(sockfd, buf, count);
}
///
// shutdown func
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
int     sockets::setSocketNonBlocking(int sockfd) {

}
void    sockets::setSocketNodelay(int sockfd, bool on = true);
void    sockets::setSocketNoLinger(int sockfd);
void    sockets::setReuseAddr(int sockfd, bool on = true);
void	sockets::setKeepAlive(int sockfd, bool on = true);
///
// socket func
int sockets::bindOrDie(int sockfd, const struct sockaddr *addr) {
    int ret = ::bind(sockfd, addr, static_cast<socklen_t>(sizeof(addr));
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