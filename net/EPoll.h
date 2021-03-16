/*
 * @Author: your name
 * @Date: 2021-03-02 17:21:31
 * @LastEditTime: 2021-03-16 15:15:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EPoll.h
 */
#ifndef _EPOLL_H_
#define _EPOLL_H_

#include "../base/Logging.h"
#include "../base/Timestamp.h"
#include <vector>

class EventLoop;
class Channel;

struct epoll_event;

class EPoll{
public:
  EPoll(EventLoop *loop);  //epoll in loop 
  ~EPoll();
  typedef std::vector<Channel*> ChannelList;
  Timestamp poll(int timeoutMs, ChannelList* activeChannels);
  void updateChannel(Channel* channel);
  void removeChannel(Channel* channel);
  
private:
  static const int kInitEventListSize = 16;//初始时最多监听事件个数
  void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;//填充就绪的channel
  void update(int operation, Channel* channel);
  typedef std::vector<struct epoll_event> EventList;
  EventLoop* ownerLoop_;	// epoller所属EventLoop, one EventLoop only has one Epoll(nulti reactor models)
  int epollfd_;				    //表示epoll的文件描述符
  EventList events_;      // Ready events container(epoll_wait)
};



#endif