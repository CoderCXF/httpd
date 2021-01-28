/*
 * @Author: your name
 * @Date: 2021-01-12 14:32:20
 * @LastEditTime: 2021-01-26 10:58:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Threadpool.h
 */
#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "Base.h"
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"
#include <functional>
#include <vector>
#include <memory.h>
#include <deque>

class Threadpool{
    
public:
    typedef std::function<void ()> TaskFunc;  // define void func() template

    explicit Threadpool(int numTasks);
    ~Threadpool();
    void start(int numThreads);
    void stop();
    void add(TaskFunc task);

private:
    TaskFunc take();
    bool isFull();
    void runInThread();

    uint32_t maxQueueSize_;   // task queue
    bool running_;
    Mutex mutex_;
    Condition notEmpty_; // taskqueue_
    Condition notFull_;
    std::vector<std::unique_ptr<Thread>> threads_;
    std::deque<TaskFunc> task_queue_;
};

#endif