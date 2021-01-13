/*
 * @Author: your name
 * @Date: 2021-01-07 11:06:36
 * @LastEditTime: 2021-01-07 19:07:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/epoll.h
 */
#ifndef _EPOLL_H_
#define _EPOLL_H_

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <errno.h>
#include <error.h>
#include <vector>
class Epoll
{
public:
    typedef std::vector<struct epoll_event> vecActiveEventType;
    Epoll(/* args */);
    ~Epoll();
    Epoll(const Epoll &epfd) = delete;
    Epoll& operator=(const Epoll &epfd) = delete;
    int EpollCtl(int fd, int op, uint32_t event);
    int EpollDispatch(vecActiveEventType & activeEvents, int timeout);
private:
    /* data */
    int m_epfd;
};

#endif
