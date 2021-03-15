/*
 * @Author: your name
 * @Date: 2021-01-07 20:00:52
 * @LastEditTime: 2021-03-14 10:40:58
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
/*

Thread::Thread(ThreadFunc func) 
	: isStarted_(false),
	  isJointed_(false), 
	  pthreadId_(0), 
	  func_(func),
	  tid_(0)
{
	
}

Thread::~Thread()
{
	assert(isStarted_ && !isJointed_);
	pthread_detach(pthreadId_);
}

struct ThreadData
{
	typedef typename Thread::ThreadFunc ThreadFunc;
	
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

void* thread_adapter(void* arg) 
{
	ThreadData* data = static_cast<ThreadData*>(arg); //this
	data->runInThread();
	data = nullptr;
	return NULL;
}

void Thread::start()
{
	assert(!isStarted_);
	printf("child thread start\n");
	// ThreadData is an object
	// if ThreadData is not exist, ptData == NULL
	printf("##################");
	std::unique_ptr<ThreadData> ptData(new ThreadData(func_, &tid_));
	if (pthread_create(&pthreadId_, NULL, thread_adapter, static_cast<void *>(ptData.get())) != 0)  {
		isStarted_ = false;
		printf("create thread fail!\n");
		exit(-1);
	}
	printf("child thread stop\n");															 
	isStarted_ = true;
}

void Thread::join()
{
	assert(isStarted_);
	pthread_join(pthreadId_, NULL);
	isJointed_ = true;
}
*/

Thread::Thread(CallFunc func) : m_bStarted(false), m_bJointed(false), m_pthreadId(0), m_func(func)
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
	typedef typename Thread::CallFunc CallFunc;
	
	ThreadData(CallFunc func) : m_func(func) {}
	
	CallFunc m_func;
};

void * ThreadRun(void * arg)
{
	ThreadData * ptData = static_cast<ThreadData *> (arg);
	ptData->m_func();
	delete ptData;
	return NULL;
}

void Thread::start()
{
	if (!m_bStarted)
	{
		ThreadData * ptData = new ThreadData(m_func);
		pthread_create(&m_pthreadId, NULL, ThreadRun, static_cast<void *> (ptData));
		m_bStarted = true;
	}
}

void Thread::join()
{
	if (!m_bJointed)
	{
		pthread_join(m_pthreadId, NULL);
		m_bJointed = true;
	}
}

