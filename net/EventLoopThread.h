/*
 * @Author: your name
 * @Date: 2021-03-16 18:09:29
 * @LastEditTime: 2021-03-16 20:03:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EventLoopThread.h
 */

#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "../base/Thread.h"
#include "../base/Mutex.h"
#include "../base/Condition.h"
#include <functional>

class EventLoop;

class EventLoopThread{
public: 
    EventLoopThread(const std::string name);
    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();
    
    EventLoop *loop_;
    bool exiting_;
    Thread thread_;
    Mutex mutex_;
    Condition cond_;
};

#endif