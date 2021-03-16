// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "EPoll.h"
#include "Channel.h"
namespace
{
const int kNew = -1;//channel还未添加
const int kAdded = 1;//channel现在是已经被添加的状态
const int kDeleted = 2;//channel现在是已经被删除的状态
}
// RAII
Poller::Poller(EventLoop* loop)
  : ownerLoop_(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize);
{
  if (epollfd_ < 0) {
    LOG_FATAL << "epoll_create error"
  }
}

// RAII
Poller::~Poller() {
  close(epoll_fd);
}

TimeStamp EPoll::poll(int timeoutMs, ChannelList* activeChannels) {
  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
  TimeStamp now(Timestamp::now());
  if (numEvents > 0) {
    LOG_TRACE << numsEvents << " events happened";
    fillActiveChannels(numEvents, activeChannels);
    // 扩容
    if (static_cast<size_t>(numEvents) == events_.size()) {
      events_.resize(events_.size() * 2);
    }
  } else if (numEvents == 0) {
    LOG_TRACE << "nothing happend";
  } else {
    LOG_SYSERR << "EPOLL::poll";
  }
  return now;
}

void EPoll::fillActiveChannels(int numEvents, ChannelList* activeChannels) {
  assert(implicit_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);//获取就绪的channel
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}

// EPOLL_CTL_ADD 或者 EPOLL_CTL_MOD
void EPoll::updateChannel(Channel *channel) {
  ownerLoop_->assertInLoopThread();
  const int index = channel->index();
}

// EPOLL_CTL_DEL
void EPoll::removeChannel(Channel *channel) {

}

void EPoll::update(int operation, Channel *channel) {
  struct epoll_event event;
  memZero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();

  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
  {
    char errMessage[32]; 
	  sprintf(errMessage,"epoll_ctl op=%d,fd=%d", operation, fd);
    LOG_SYSFATAL << errMessage;
  }
}

bool Poller::hasChannel(Channel* channel) const
{
  ownerLoop->assertInLoopThread();
  ChannelMap::const_iterator it = channels_.find(channel->fd());
  return it != channels_.end() && it->second == channel;
}

