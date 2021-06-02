/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:37
 * @LastEditTime: 2021-06-02 16:30:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.cpp
 */
#include <poll.h>
#include <sys/eventfd.h>
#include <signal.h>
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
      timerQueue_(new TimerQueue(this)),
      wakeupFd_(createEventfd()),
      wakeupChannel_(new Channel(this, wakeupFd_)),
      currentActiveChannel_(NULL)
{
    // TODO: ignore SIGPIPE signal
    signal(SIGPIPE,SIG_IGN);  
    createPipe(fd_);
    LOG_TRACE << "EventLoop " << this << " created in this thread " << threadId_;
    if (t_loopInThisThread) {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
                  << " in current thread " << threadId_;
    } else {
        t_loopInThisThread = this;
    }
    wakeupChannel_->setReadCallback(
      std::bind(&EventLoop::handleRead, this));
    // we are always reading the wakeupfd
    wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
    t_loopInThisThread = NULL;
}

void EventLoop::createPipe(int fd[2]) {
  int ret = ::pipe(fd);
  if (ret != 0) {
    LOG_SYSFATAL << "EvetnLoop::createPipe";
  }
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
        doPendingFunctors();//让I/O线程也能处理一些计算任务
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
  // 将任务添加到队列中
  {
    MutexGuard lock(mutex_);
    pendingFunctors_.push_back(std::move(cb));
  }
  // 调用queueInLoop的线程不是当前线程，需要唤醒
  // 或者调用queueInLoop的线程是当前IO线程，并且此时正在调用pending functor,需要唤醒
  // 只有当前IO线程的事件回调中queueInLoop才不需要唤醒
  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}

void EventLoop::wakeup()
{
  // //只有固定的8字节（64位）的缓冲区
  
  uint64_t one = 1;
  ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
  
 // close read;
//  close(fd_[0]);
//  int flags = fcntl(fd_[0], F_GETFL, 0);
//   flags |= O_NONBLOCK;
//   fcntl(fd_[0], F_SETFL, flags);
//  uint32_t buf = 1;
//  ssize_t nbytes = sockets::write(fd_[1], &buf, sizeof buf);
//  if (nbytes != sizeof buf) {
//    LOG_ERROR << "EventLoop::wakeup";
//  }
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

void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
  // close write
  // close(fd_[1]);
  // int flags = fcntl(fd_[1], F_GETFL, 0);
  // flags |= O_NONBLOCK;
  // fcntl(fd_[1], F_SETFL, flags);
  // uint32_t buf = 1;
  // ssize_t nbytes = sockets::read(fd_[0], &buf, sizeof buf);
  // if (nbytes != sizeof buf) {
  //   LOG_SYSFATAL << "EventLoop::handleRead";
  // }
}
void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
    MutexGuard lock(mutex_);
    functors.swap(pendingFunctors_);
  }

  for (const Functor& functor : functors)
  {
    functor();
  }
  callingPendingFunctors_ = false;
}


TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
  return timerQueue_->addTimer(std::move(cb), time, 0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallback cb)
{
  Timestamp time(addTime(Timestamp::now(), delay));
  return runAt(time, std::move(cb));
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
  Timestamp time(addTime(Timestamp::now(), interval));
  return timerQueue_->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
  return timerQueue_->cancel(timerId);
}
