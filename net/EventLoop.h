/*
 * @Author: your name
 * @Date: 2021-03-02 19:47:27
 * @LastEditTime: 2021-03-15 21:04:59
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

// #include <boost/any.hpp>

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
  EventLoop();
  ~EventLoop();
  void loop();
  void assertInLoopThread() { //确定该loop在当前线程当中
    if (!isInLoopThread()) {
      abortNotInLoopThread();
    }
  }
  bool isInLoopThread() const{
    return CurrentThread::tid() == threadId_
  }
  //->EPoll::removeChannel()->epoll_ctl(, EPOLL_CTL_DEL, channel->fd(), channel->events())
  void removeChannel(Channel *channel);   
  void updateChannel(Channel *channel);
  bool hasChannel(Channel *channel);

private:
  void abortNotInLoopThread();
  bool looping_;
  const pid_t threadId_; // thread is that create loop thread
  std::shared_ptr<EPoll> poll;
};

#endif  // MUDUO_NET_EVENTLOOP_H
