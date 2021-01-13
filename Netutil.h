/*
 * @Author: your name
 * @Date: 2021-01-06 10:41:40
 * @LastEditTime: 2021-01-07 15:27:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/netuntil.h
 */
#ifndef _NETUTIL_H_
#define _NETUTIL_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

class NetUtil{
public:
    static int Listen(const char *serverIP, uint16_t port);
    static int Connect(const char *serverIP, uint16_t port);
private:
    static void SetReuseAddr(int fd, int opt = 1);
    static void SetNonblock(int fd);
    static void SetAddr(const char * serverIp, uint16_t port, sockaddr_in & sockAddr);
};

#endif