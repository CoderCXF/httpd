/*
 * @Author: your name
 * @Date: 2021-03-16 18:09:39
 * @LastEditTime: 2021-03-16 20:07:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EventLoopThread.cpp
 */
#include "EventLoopThread.h"
#include "EventLoop.h"

EventLoopThread::EventLoopThread(const std::string name)
    : loop_(NULL),
      exiting_(false),
      thread_(std::bind(&EventLoopThread::threadFunc, this)),
      mutex_(),
      cond_(mutex_)
{
    
}

EventLoopThread::~EventLoopThread() {
    exiting_ = true;
    if (loop_ != NULL) {
        loop_->quit();
        thread_.join();
    }
}

// 这个函数会返回新线程（IO线程）中的loop指针，所以使用条件变量阻塞等待新的线程得到loop
EventLoop* EventLoopThread::startLoop() {
    assert(!thread_.started());
    thread_.start();
    {
        MutexGuard lock(mutex_);
        while (loop_ == NULL) {
            cond_.Wait();
        }
    }
    return loop_;
}

void EventLoopThread::threadFunc() {
    EventLoop loop;
    {
        MutexGuard lock(mutex_);
        loop_ = &loop;
        cond_.Notify();
    }
    loop.loop();
    loop_ = NULL;
}

