/*
 * @Author: your name
 * @Date: 2021-01-09 14:57:27
 * @LastEditTime: 2021-01-25 15:44:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Mutex.cpp
 */
#include "Mutex.h"
Mutex::Mutex() {
    pthread_mutex_init(&mutex_, NULL);
}
Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex_);
}
void Mutex::lock() {
    pthread_mutex_lock(&mutex_);
}

void Mutex::unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *Mutex::getMutexInstance() {
    return &mutex_;
}

/*RAII*/
MutexGuard::MutexGuard(Mutex &mutex):m_Mutex(mutex){
    m_Mutex.lock();
}

MutexGuard::~MutexGuard() {
    m_Mutex.unlock();
}