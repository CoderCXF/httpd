/*
 * @Author: your name
 * @Date: 2021-03-17 09:42:38
 * @LastEditTime: 2021-03-17 19:35:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/SocketOps.h
 */
#pragma once
#include <arpa/inet.h>
// #include"../base/noncopyable.h"

namespace sockets{
    ssize_t read(int sockfd, void *buf, size_t count);
    ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
    ssize_t write(int sockfd, const void *buf, size_t count);
    void    close(int sockfd);
    void    shutdownRead(int sockfd);
    void    shutdownWrite(int sockfd);
    void    shutdownRDWR(int sockfd);

    ///
    //NON_BLOCK
    ///
    int     setSocketNonBlocking(int sockfd);
    ///
    // SO_REUSEADDR
    ///
    void    setReuseAddr(int sockfd, bool on = true);
    ///
    // TCP_NODELAY(nagle)
    ///
    void    setTcpNoDelay(int sockfd, bool on);
    ///
    // SO_KEEPALIVE
    ///
    void	setKeepAlive(int sockfd, bool on = true);
    ///
    // 
    ///
    // void    setSocketNodelay(int sockfd, bool on = true);
    // void    setSocketNoLinger(int sockfd);
    
    int bindOrDie(int sockfd, const struct sockaddr *addr);
    int listenOrDie(int sockfd);
    int  accept(int sockfd, struct sockaddr* addr);
    int  connect(int sockfd, const struct sockaddr* addr);

}
