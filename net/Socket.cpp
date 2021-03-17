/*
 * @Author: your name
 * @Date: 2021-03-17 10:18:07
 * @LastEditTime: 2021-03-17 11:38:15
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
    if (close(sockfd_) < 0) {
        LOG_FATAL << "Socket::~Socket error";
    }
}

void Socket::Bind(const struct sockaddr *addr) {
    sockets::bindOrDie(sockfd_, addr);
}

void Socket::Listen() {
    sockets::listenOrDie(sockfd_);
}