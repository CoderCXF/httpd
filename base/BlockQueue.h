/*
 * @Author: your name
 * @Date: 2021-01-22 09:47:45
 * @LastEditTime: 2021-03-14 09:14:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/BlockQueue.h
 */
#ifndef _BLOCKQUEUE_H_
#define _BLOCKQUEUE_H_

#include "Base.h"
#include "Mutex.h"
#include "Condition.h"
#include <deque>

template <typename T>
class BlockQueue{
public:
    BlockQueue()
        : mutex_(),
          notEmpty_(mutex_),
          queue_()
    {
    }
    void push(const T& x) {
        MutexGuard lock(mutex_);
        queue_.push_back(x);
        notEmpty_.Notify();
    }
    // like consumer : judge notEmpty
    T take() {
        MutexGuard lock(mutex_);
        while (queue_.empty()) {
            notEmpty_.Wait();
        }
        T front(std::move(queue_.front()));
        queue_.pop_front();
        return front;
    }
    
    size_t size() const{
        MutexGuard lock(mutex_);
        return queue_.size();
    }
private:
    mutable Mutex mutex_;
    Condition notEmpty_;
    std::deque<T> queue_;
};

#endif