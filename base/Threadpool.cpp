/*
 * @Author: your name
 * @Date: 2021-01-12 14:32:31
 * @LastEditTime: 2021-01-12 17:23:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Threadpool.cpp
 */
#include "Threadpool.h"
#include <assert.h>

Threadpool::Threadpool(int numTasks)
    : maxQueueSize_(numTasks),
      running_(false),
      mutex_(),
      notEmpty_(mutex_),
      notFull_(mutex_)
{

}

Threadpool::~Threadpool() {
    if (running_) {
        stop();
    }
}

void Threadpool::start(int numThreads) {
    assert(!running_);
    running_ = true;
    for (int i = 0; i < numThreads; ++i) {
        threads_.emplace_back(new Thread(std::bind(&Threadpool::runInThread, this)));
        threads_[i]->Start();
    }
}

void Threadpool::stop() {
    while (running_) {
        running_ = false;
        notEmpty_.NotifyAll();
        for (std::unique_ptr<Thread> & item : threads_)
        {
            item->Join();
        }
        
        std::vector<std::unique_ptr<Thread>> temp;
        threads_.swap(temp);
    }
}


bool Threadpool::isFull() {
    return task_queue_.size() == maxQueueSize_;
}

void Threadpool::add(TaskFunc task) {
    // no thread for task, run task directly
    if (threads_.empty()) {  
        task();
    } else {
        MutexGuard lock(mutex_);
        while (isFull() && running_) {
            notFull_.Wait();
        }
        task_queue_.push_back(task);
        notEmpty_.Notify();
    }
}

// cond:notEmpty && notFull
Threadpool::TaskFunc Threadpool::take() {
    MutexGuard lock(mutex_);
    while (task_queue_.empty() && running_) {
        notEmpty_.Wait();  // wait for not empty
    }
    TaskFunc task;
    if (!task_queue_.empty()) {
        task = task_queue_.front();
        task_queue_.pop_front();
        notFull_.Notify();
    }
    return task;
}

void Threadpool::runInThread() {
    while (running_) {
        TaskFunc task = take();
        if (task) {  // task is a function name
            task();
        }
    }
}