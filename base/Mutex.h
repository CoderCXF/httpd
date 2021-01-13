/*
 * @Author: your name
 * @Date: 2021-01-09 14:57:19
 * @LastEditTime: 2021-01-11 16:26:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Mutex.h
 */
#ifndef _MUTEX_H_
#define _MUTEX_H_
#include <unistd.h>
#include <pthread.h>

class Mutex{
public:
    Mutex();
    ~Mutex();
    // Mutex(const Mutex &) = delete; //constructor function initilizer list
    Mutex& operator=(const Mutex &) = delete;
public:
    void Lock();
    void Unlock();
    pthread_mutex_t *GetMutexInstance();

private:
    pthread_mutex_t mutex_;
};

class MutexGuard{
public:
    explicit MutexGuard(Mutex &mutex);
    ~MutexGuard();

private:
    Mutex &m_Mutex;
};
// prevent issue occur
// MutexGuard(mutex)
// A tempory object doesn't hold the lock for long!
// #define MutexGuard(x) error "Missing guard object name"
#endif