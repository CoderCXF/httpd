/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:37
 * @LastEditTime: 2021-03-24 12:01:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.cpp
 */
#include <poll.h>
#include <sys/eventfd.h>
#include "EventLoop.h"
#include "Channel.h"
#include "EPoll.h"
#include "../base/Logging.h"
#include "SocketOps.h"

// one loop only belone to one thread;
// In other words, one thread only one eventloop object
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() 
    : looping_(false),
      quit_(false),
      eventHandling_(false),
      callingPendingFunctors_(false),
      threadId_(CurrentThread::tid()),
      poller_(std::make_shared<EPoll>(this)), // create poller_ by call EPoll constructor(epoll_create)
      wakeupFd_(createEventfd()),
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

void EventLoop::runInLoop(Functor cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(std::move(cb));
  }
}

void EventLoop::queueInLoop(Functor cb)
{
  {
    MutexGuard lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}

int EventLoop::createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

