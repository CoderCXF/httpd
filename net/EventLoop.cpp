/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:37
 * @LastEditTime: 2021-03-15 16:25:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.cpp
 */
#include <poll.h>
#include "EventLoop.h"
#include "../base/Logging.h"

// one loop only belone to one thread;
// In other words, one thread only one eventloop object
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() 
    : looping_(false),
      threadId_(CurrentThread::tid()),
      poll(make_shared<EPoll>(this))
{
    LOG_TRACE << "EventLoop created" << this << " in this thread " << threadId_;
    if (t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
                  << " in current thread " << threadId_;
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
    LOG_TRACE << "EventLoop:" << this << " start looping";
    ::poll(NULL, 0, 5000); // current eventloop only wait 5s
    LOG_TRACE << "EventLoop:" << this << " stop looping";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << CurrentThread::tid();
}

void removeChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    //TODO:
    poll->removeChannel(channel);
}

void updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    //TODO:
    poll->updateChannel(channel);
}

bool hasChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    return poll->hashChannel(channel);
}


