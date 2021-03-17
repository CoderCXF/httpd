/*
 * @Author: your name
 * @Date: 2021-03-17 09:42:38
 * @LastEditTime: 2021-03-17 11:46:08
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
    // void    close();
    void    shutdownRead(int sockfd);
    void    shutdownWrite(int sockfd);
    void    shutdownRDWR(int sockfd);

    int     setSocketNonBlocking(int sockfd);
    void    setSocketNodelay(int sockfd, bool on = true);
    void    setSocketNoLinger(int sockfd);
    void    setReuseAddr(int sockfd, bool on = true);
    void	setKeepAlive(int sockfd, bool on = true);
    
    int bindOrDie(int sockfd, const struct sockaddr *addr);
    int listenOrDie(int sockfd);
}
