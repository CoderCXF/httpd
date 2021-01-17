/*
 * @Author: your name
 * @Date: 2021-01-07 20:00:52
 * @LastEditTime: 2021-01-17 15:03:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Thread.cpp
 */
#include "Thread.h"

pid_t MyFuncSpace::gettid() {
	return static_cast<pid_t>(syscall(SYS_gettid));
}

__thread int CurrentThread::t_cachedTid = 0;

pid_t CurrentThread::tid() {
	if (t_cachedTid == 0) {
		t_cachedTid = MyFuncSpace::gettid();
	}
	return t_cachedTid;
}

Thread::Thread(ThreadFunc func) 
	: m_bStarted(false),
	  m_bJointed(false), 
	  m_pthreadId(0), 
	  func_(func),
	  tid_(0)
{
	
}

Thread::~Thread()
{
	if (m_bStarted && !m_bJointed)
	{
		pthread_detach(m_pthreadId);
	}
}

struct ThreadData
{
	typedef typename Thread::ThreadFunc ThreadFunc;
public:
	ThreadFunc func_;
	pid_t* tid_;
	ThreadData(ThreadFunc func, pid_t *tid) 
		: func_(std::move(func)),
		  tid_(tid) 
	{

	}
	// member function
	void runInThread() {
		*tid_ = CurrentThread::tid();
		tid_ = nullptr;
		func_();
	}

};

void* thread_adapter(void* obj)
{
	ThreadData* data = static_cast<ThreadData*>(obj);
	data->runInThread();
	delete data;
	return NULL;
}

void Thread::Start()
{
	if (!m_bStarted)
	{
		ThreadData * ptData = new ThreadData(func_, &tid_);
		pthread_create(&m_pthreadId, NULL, thread_adapter, static_cast<void *> (ptData)); // ThreadData is an object
																						  // if ThreadData is not exist, ptData == NULL
		m_bStarted = true;
	}
}

void Thread::Join()
{
	if (!m_bJointed)
	{
		pthread_join(m_pthreadId, NULL);
		m_bJointed = true;
	}
}
