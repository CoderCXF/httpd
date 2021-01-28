/*
 * @Author: your name
 * @Date: 2021-01-09 21:00:44
 * @LastEditTime: 2021-01-26 10:27:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/condition.h
 */
#ifndef _CONDITION_H_
#define _CONDITION_H_

#include <pthread.h>
#include "Base.h"
#include "Mutex.h"

class Condition{
public:
    explicit Condition(Mutex &mutex);
    ~Condition();
    void Wait();
    bool WaitForSeconds(double seconds);
    void Notify();
    void NotifyAll();
    
private:
    pthread_cond_t cond_;
    Mutex &mutex_;
};


#endif