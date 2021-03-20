/*
 * @Author: your name
 * @Date: 2021-03-16 09:26:42
 * @LastEditTime: 2021-03-19 14:34:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EPoll.cpp
 */

#include "EPoll.h"
#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>
namespace
{
const int kNew = -1;//channel还未添加
const int kAdded = 1;//channel现在是已经被添加的状态
const int kDeleted = 2;//channel现在是已经被删除的状态
}
// RAII
EPoll::EPoll(EventLoop* loop)
  : ownerLoop_(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize)
{
  if (epollfd_ < 0) {
    LOG_FATAL << "epoll_create error";
  }
}

// RAII
EPoll::~EPoll() {
  close(epollfd_);
}

Timestamp EPoll::poll(int timeoutMs, ChannelList* activeChannels) {
  int numEvents = ::epoll_wait(epollfd_, &*events_.begin(), static_cast<int>(events_.size()), timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0) {
    LOG_TRACE << numEvents << " events happened";
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

void EPoll::fillActiveChannels(int numEvents, ChannelList* activeChannels) const{
  assert(static_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr); //获取就绪的channel
    // Set revents for channel
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}
// private function
void EPoll::update(int operation, Channel *channel) {
  struct epoll_event event;
  memset(&event, 0, sizeof event);  //or bzero
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

// EPOLL_CTL_ADD 或者 EPOLL_CTL_MOD
void EPoll::updateChannel(Channel *channel) {
  ownerLoop_->assertInLoopThread();
  const int index = channel->index();
  // new or deleted
  if (index == kNew || index == kDeleted) {
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  // existing
  else {
    assert(index == kAdded);
    if (channel->isNoneEvent())//从epoll上删除channel
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    }
    else//在epoll上修改channel
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}

// EPOLL_CTL_DEL
void EPoll::removeChannel(Channel *channel) {
  ownerLoop_->assertInLoopThread();
  assert(channel->isNoneEvent());
  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  //调用了updateChannel和removeChannel,后状态才会变为kNew，直接调用removeChannel会断言失败
  channel->set_index(kNew);//将该channel设置成未添加
}
