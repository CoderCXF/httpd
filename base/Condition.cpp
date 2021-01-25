/*
 * @Author: your name
 * @Date: 2021-01-09 21:01:22
 * @LastEditTime: 2021-01-24 10:46:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/condition.cpp
 */
#include "Condition.h"
// #include <time.h>
#include <sys/time.h>
Condition::Condition(Mutex &mutex) : mutex_(mutex) {
    pthread_cond_init(&cond_, NULL);
}

Condition::~Condition() {
    pthread_cond_destroy(&cond_);
}
void Condition::Wait() {
    pthread_cond_wait(&cond_, mutex_.GetMutexInstance());
}
bool Condition::WaitForSeconds(double seconds) {
    struct timespec wait_time;
    clock_gettime(CLOCK_MONOTONIC, &wait_time);
    int nonsecondsOfSecond = 1000000000;
    wait_time.tv_sec = static_cast<time_t>(wait_time.tv_sec + seconds);
    wait_time.tv_nsec = static_cast<long>(wait_time.tv_nsec + seconds * nonsecondsOfSecond);
    // ETIMEDOUT : errno = 110
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.GetMutexInstance(), &wait_time); 
}

void Condition::Notify() {
    pthread_cond_signal(&cond_);
}
void Condition::NotifyAll() {
    pthread_cond_broadcast(&cond_);
}
