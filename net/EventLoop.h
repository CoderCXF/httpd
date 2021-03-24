/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:27
 * @LastEditTime: 2021-03-24 09:40:02
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

#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Timestamp.h"

class Channel;
class EPoll;
//
// Reactor, one loop one thread
//

class EventLoop{
public:
  typedef std::function<void()> Functor;
  EventLoop();
  ~EventLoop();
  void loop();
  void runInLoop(Functor cb);
  void queueInLoop(Functor cb);
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

  bool looping_;
  bool quit_;
  bool eventHandling_;
  bool callingPendingFunctors_; /* atomic */
  const pid_t threadId_; // thread is that create loop thread
  std::shared_ptr<EPoll> poller_;
  int wakeupFd_;
  Channel* currentActiveChannel_;
  ChannelList activeChannels_;
  Timestamp pollReturnTime_;

  // TODO:
  Mutex mutex_;
  std::vector<Functor> pendingFunctors_;
  static const int kPollTimeMs = 10000;
};

#endif  // MUDUO_NET_EVENTLOOP_H
