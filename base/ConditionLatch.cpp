/*
 * @Author: your name
 * @Date: 2021-01-11 15:35:47
 * @LastEditTime: 2021-01-11 15:58:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/ConditionLatch.cpp
 */
#include "ConditionLatch.h"
ConditionLatch::ConditionLatch(int count)
    :mutex_(),
     cond_(mutex_),
     count_(count)
{

}

void ConditionLatch::Wait() {
    MutexGuard lock(mutex_);
    while (count_ > 0) {
        cond_.Wait();
    }
}

void ConditionLatch::CountDown() {
    MutexGuard lock(mutex_);
    --count_;
    if (count_ == 0) {
        cond_.NotifyAll();
    }
}

int ConditionLatch::GetCount() {
    MutexGuard lock(mutex_);
    return count_;
}