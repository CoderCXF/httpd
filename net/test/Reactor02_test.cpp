/*
 * @Author: your name
 * @Date: 2021-03-16 11:35:13
 * @LastEditTime: 2021-03-16 15:10:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/test/Reactor02_test.cpp
 */
#include <sys/timerfd.h>

#include "../EventLoop.h"
#include "../Channel.h"
#include <string.h>

int timerfd;

EventLoop *g_loop;
void timeout(Timestamp receiveTime) {
    printf("timeout!\n");
    uint64_t howmany;
    ssize_t ret;
    ret = read(timerfd, &howmany, sizeof howmany);
    if (ret < 0) {
        return;
    }
    // g_loop->quit();
}

int main() {
    EventLoop loop;
    g_loop = &loop;

    timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop, timerfd);
    channel.setReadCallback(std::bind(timeout, std::placeholders::_1));
    channel.enableReading();
    
    struct itimerspec howlong;
    memset(&howlong, 0, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    timerfd_settime(timerfd, 0, &howlong, NULL);
    loop.loop();

    close(timerfd);
}
