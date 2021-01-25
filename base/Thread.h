/*
 * @Author: your name
 * @Date: 2021-01-07 20:00:52
 * @LastEditTime: 2021-01-25 15:01:12
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Thread.h
 */
#ifndef _THREAD_H_
#define _THREAD_H_
// #define _GNU_SOURCE
#include "Base.h"
#include <pthread.h>
#include <assert.h>
#include <functional>
#include <memory>
#include <sys/prctl.h>
#include <sys/syscall.h>
// only once
namespace MyFuncSpace
{
	// only declaration, not definition
	pid_t gettid();
} // namespace myFunc

class CurrentThread {
public:
	static __thread int t_cachedTid;
	static pid_t tid();
};
/*
 * memeber function:
 * Start() Join() tid()
 */

class Thread
{
public:
	typedef std::function<void ()> ThreadFunc;
	
	explicit Thread(ThreadFunc func);
	Thread(const Thread &) = delete;
	Thread & operator= (const Thread &) = delete;
	~Thread();
	
	void start();
	void join();
	inline pid_t tid() { return tid_; }

private:
	bool isStarted_;
	bool isJointed_;
	pthread_t pthreadId_;
	ThreadFunc func_;
	pid_t tid_; // pid_t == int
};



#endif