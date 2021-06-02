/*
 * @Author: your name
 * @Date: 2021-06-02 10:22:09
 * @LastEditTime: 2021-06-02 15:21:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit  
 * @FilePath: /httpd/net/Timer.cpp
 */
// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef TIMER_H
#define TIMER_H

// #include "/base/Atomic.h"
#include "../base/Atomic.h"
#include "../base/Timestamp.h"
#include <functional>
// #include "net/Callbacks.h"
///
/// Internal class for timer event.
///
class Timer
{
 public:
   typedef std::function<void()> TimerCallback;
   
   Timer(TimerCallback cb, Timestamp when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet())
  { }

  void run() const
  {
    callback_();
  }

  Timestamp expiration() const  { return expiration_; }
  bool repeat() const { return repeat_; }
  int64_t sequence() const { return sequence_; }

  void restart(Timestamp now);

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  const TimerCallback callback_;  // 
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
  const int64_t sequence_;

  static AtomicInt64 s_numCreated_;
};

#endif  // MUDUO_NET_TIMER_H
