/*
 * @Author: your name
 * @Date: 2021-03-03 14:51:45
 * @LastEditTime: 2021-03-15 16:16:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/test/EventLoop_test.cpp
 */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "../EventLoop.h"
#include "../../base/Thread.h"

using namespace std;

//test1
// void threadFunc() {
//     printf("Child thread pid = %d, Thread tid = %d\n", getpid(), CurrentThread::tid());
//     EventLoop loop;
//     loop.loop();
// }

// int main() {
//     printf("Main thread pid = %d, Thread tid = %d\n", getpid(), CurrentThread::tid());
//     EventLoop loop;
//     Thread t(threadFunc);
//     t.start();
//     loop.loop();
//     t.join();
//     return 0;
// }

//test2
EventLoop *g_loop;
void threadFunc() {
    // printf("Child thread pid = %d, Thread tid = %d\n", getpid(), CurrentThread::tid());
    g_loop->loop();
}
int main() {
    // printf("Main thread pid = %d, Thread tid = %d\n", getpid(), CurrentThread::tid());
    EventLoop loop;
    g_loop = &loop;
    Thread t(threadFunc);
    t.start();
    g_loop->loop();
    t.join();
    return 0;
}
