/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:37
 * @LastEditTime: 2021-03-03 14:53:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.cpp
 */
#include <poll.h>
#include "EventLoop.h"
#include "../base/Logging.h"

// one loop only belone to one thread;
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() 
    : looping_(false),
      threadId_(CurrentThread::tid())
{
    LOG_TRACE << "EventLoop created" << this << "in this thread " << threadId_;
    if (!t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop" << t_loopInThisThread
                  << "in current thread" << threadId_;
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop() {
    t_loopInThisThread = NULL;
}

void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    LOG_TRACE << "EventLoop:" << this << "start looping";
    poll(NULL, 0, 5000); // current eventloop only wait 5s
    LOG_TRACE << "EventLoop:" << this << "stop looping";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << CurrentThread::tid();
}