/*
 * @Author: your name
 * @Date: 2021-01-11 15:35:35
 * @LastEditTime: 2021-01-11 15:52:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/ConditionLatch.h
 */
#include "Base.h"
#include "Condition.h"
#include "Mutex.h"
class ConditionLatch{
public:
    ConditionLatch(int count);
    ~ConditionLatch() = default;
    // All child threads wait block
    void Wait();    
    // Main thread notify all child threads if count = 0
    void CountDown();
    int GetCount();
private:
    Mutex mutex_;
    Condition cond_;
    int count_;
};