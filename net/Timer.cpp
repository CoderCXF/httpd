/*
 * @Author: your name
 * @Date: 2021-06-02 10:22:09
 * @LastEditTime: 2021-06-02 15:22:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /httpd/net/Timer.cpp
 */
#include "Timer.h"


AtomicInt64 Timer::s_numCreated_;

// 重启定时器，如果是一个重复的定时器，那么下一次的超时时间就是now + interval_
// 如果不是一个重复的定时器的话，超时时间就是无效的
void Timer::restart(Timestamp now)
{
  if (repeat_)
  {
    expiration_ = addTime(now, interval_);
  }
  else
  {
    expiration_ = Timestamp::invalid();
  }
}