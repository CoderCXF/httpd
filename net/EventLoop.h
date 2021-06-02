/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:27
 * @LastEditTime: 2021-06-02 16:31:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.h
 */

#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <atomic>
#include <functional>
#include <vector>
#include <memory>
#include <assert.h>
#include <fcntl.h>

#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Timestamp.h"
#include "TimerId.h"
#include "TimerQueue.h"

class Channel;
class EPoll;
//
// Reactor, one loop one thread
//

class EventLoop{
public:
  typedef std::function<void()> Functor;
  typedef std::function<void()> TimerCallback;
  EventLoop();
  ~EventLoop();
  void loop();
  void runInLoop(Functor cb);
  void queueInLoop(Functor cb);

    // timers

  ///
  /// Runs callback at 'time'.
  /// Safe to call from other threads.
  ///
  TimerId runAt(Timestamp time, TimerCallback cb);
  ///
  /// Runs callback after @c delay seconds.
  /// Safe to call from other threads.
  ///
  TimerId runAfter(double delay, TimerCallback cb);
  ///
  /// Runs callback every @c interval seconds.
  /// Safe to call from other threads.
  ///
  TimerId runEvery(double interval, TimerCallback cb);
  ///
  /// Cancels the timer.
  /// Safe to call from other threads.
  ///
  void cancel(TimerId timerId);
  
  void wakeup();
  int createEventfd();
  
  void removeChannel(Channel *channel);   
  void updateChannel(Channel *channel);
  void quit() { quit_ = true; }

  
  void assertInLoopThread() { //确定该loop在当前线程当中
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const{
    return CurrentThread::tid() == threadId_;
  }
private:
  typedef std::vector<Channel*> ChannelList;
  void abortNotInLoopThread();
  void handleRead();
  void doPendingFunctors();
  void createPipe(int fd[2]);
  
  bool looping_;
  bool quit_;
  bool eventHandling_;
  bool callingPendingFunctors_; /* atomic */
  const pid_t threadId_; // thread is that create loop thread
  std::shared_ptr<EPoll> poller_;
  std::unique_ptr<TimerQueue> timerQueue_;
  int wakeupFd_;
  std::unique_ptr<Channel> wakeupChannel_;
  Channel* currentActiveChannel_;
  int fd_[2];
  ChannelList activeChannels_;
  Timestamp pollReturnTime_;

  // TODO:
  Mutex mutex_;
  std::vector<Functor> pendingFunctors_;
  static const int kPollTimeMs = 10000;
};

#endif  // MUDUO_NET_EVENTLOOP_H
