/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:37
 * @LastEditTime: 2021-03-16 14:26:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.cpp
 */
#include <poll.h>
#include "EventLoop.h"
#include "Channel.h"
#include "EPoll.h"
#include "../base/Logging.h"

// one loop only belone to one thread;
// In other words, one thread only one eventloop object
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() 
    : looping_(false),
      quit_(false),
      eventHandling_(false),
      threadId_(CurrentThread::tid()),
      poller_(std::make_shared<EPoll>(this)),
      currentActiveChannel_(NULL)
{
    LOG_TRACE << "EventLoop " << this << " created in this thread " << threadId_;
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
    quit_ = false;
    LOG_TRACE << "EventLoop " << this << " start looping";
    ::poll(NULL, 0, 5000); // current eventloop only wait 5s
    while (!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        eventHandling_ = true;
        for (ChannelList::iterator it = activeChannels_.begin();
            it != activeChannels_.end(); ++it)
        {
            currentActiveChannel_ = *it;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = NULL;
        eventHandling_ = false;
        // doPendingFunctors();//让I/O线程也能处理一些计算任务
    }
    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread() {
    LOG_FATAL << "EventLoop " << this << " of thread " << threadId_
              << " destructs in thread " << CurrentThread::tid();
}

void EventLoop::updateChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    //TODO:
    if (eventHandling_)
    {
        assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
    }
    poller_->removeChannel(channel);
}


