/*
 * @Author: your name
 * @Date: 2021-03-23 20:56:15
 * @LastEditTime: 2021-03-23 21:20:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/net/EventLoopThreadPool.h
 */
#ifndef EVENTLOOPTHREADPOOL_H
#define EVENTLOOPTHREADPOOL_H
#include <string>
#include <vector>
#include <memory>
class EventLoop;
class EventLoopThread;
class EventLoopThreadPool
{
public:
	EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
	~EventLoopThreadPool();
	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
	void start(bool starTimer = false);

	// round-robin
	EventLoop* getNextLoop();

private:

	EventLoop* baseloop_;										//和accept所属的eventloop相同
	std::string name_;
	bool started_;
	int next_;													//新连接到来时，所选择的EventLoop对象下标
	int numThreads_;											//线程数

	std::vector<std::unique_ptr<EventLoopThread>>threads_;		//IO线程列表
	std::vector<EventLoop*> loops_;								//EventLoop列表
	// std::vector< std::unique_ptr<TimerHeapManager>>mannagers_;	//TimerHeapManager列表
};
#endif