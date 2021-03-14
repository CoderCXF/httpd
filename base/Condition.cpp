/*
 * @Author: your name
 * @Date: 2021-01-09 21:01:22
 * @LastEditTime: 2021-03-03 15:31:11
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
    pthread_cond_wait(&cond_, mutex_.getMutexInstance());
}
bool Condition::WaitForSeconds(double seconds) {
   struct timespec abstime;
  // FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
  clock_gettime(CLOCK_REALTIME, &abstime);

  const int64_t kNanoSecondsPerSecond = 1000000000;
  int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);

  abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
  abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);

  Mutex lock(mutex_);
  return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.getMutexInstance(), &abstime);
}

void Condition::Notify() {
    pthread_cond_signal(&cond_);
}
void Condition::NotifyAll() {
    pthread_cond_broadcast(&cond_);
}
