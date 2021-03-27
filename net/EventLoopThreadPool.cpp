/*
 * @Author: your name
 * @Date: 2021-03-23 20:56:26
 * @LastEditTime: 2021-03-27 14:52:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EventLoopThreadpool.cpp
 */
#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include "../base/Logging.h"
EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg)
  : baseloop_(baseLoop),
    name_(nameArg),
    started_(false),
    numThreads_(0),
    next_(0)
{

}
EventLoopThreadPool::~EventLoopThreadPool() 
{
    started_ = false;
}

void EventLoopThreadPool::start(bool startTimer) {
    assert(!started_);
    assert(baseloop_->isInLoopThread());
    started_ = true;
    for (int i = 0; i < numThreads_; ++i) {
        char buf[name_.size() + 32];
        snprintf(buf, sizeof buf, "%s%d", name_.c_str(), i);    
        EventLoopThread *t = new EventLoopThread(buf);
        threads_.push_back(std::unique_ptr<EventLoopThread>(t));
        loops_.push_back(t->startLoop());
    }
}

EventLoop* EventLoopThreadPool::getNextLoop() {
    // for debug
    // LOG_DEBUG << "baseloop_->assertInLoopThread() Before";
    baseloop_->assertInLoopThread();
    // LOG_DEBUG << "baseloop_->assertInLoopThread() After";
    assert(started_);
    EventLoop *loop = baseloop_;
    // round-robin
    if (!loops_.empty()) {
        loop = loops_[next_];
        ++next_;
        if (static_cast<size_t>(next_) >= loops_.size()) {
            next_ = 0;
        }
    }
    return loop;
}

// TODO: hash code method