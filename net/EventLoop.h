/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:27
 * @LastEditTime: 2021-03-03 14:41:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/Eventloop.h
 */

#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_

#include <atomic>
#include <functional>
#include <vector>

// #include <boost/any.hpp>

#include "../base/Mutex.h"
#include "../base/Thread.h"
#include "../base/Timestamp.h"

class Channel;
class Poller;
//
// Reactor, one loop one thread
//

class EventLoop{
public:
  EventLoop();
  ~EventLoop();
  void loop();
  void assertInLoopThread() {
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const{
    return CurrentThread::tid() == threadId_;
  }

private:
  void abortNotInLoopThread();
  bool looping_;
  const pid_t threadId_; // thread is that create loop thread
};

#endif  // MUDUO_NET_EVENTLOOP_H
