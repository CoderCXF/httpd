/*
 * @Author: your name
 * @Date: 2021-01-09 14:57:27
 * @LastEditTime: 2021-01-11 15:01:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Mutex.cpp
 */
#include "Mutex.h"
#include <iostream>
Mutex::Mutex() {
    pthread_mutex_init(&mutex_, NULL);
}
Mutex::~Mutex() {
    pthread_mutex_destroy(&mutex_);
}
void Mutex::Lock() {
    pthread_mutex_lock(&mutex_);
}

void Mutex::Unlock() {
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t *Mutex::GetMutexInstance() {
    return &mutex_;
}
/*RAII*/
MutexGuard::MutexGuard(Mutex &mutex):m_Mutex(mutex){
    // std::cout << "constructor call" << std::endl;
    m_Mutex.Lock();
}

MutexGuard::~MutexGuard() {
    // std::cout << "deconstructor call" << std::endl;
    m_Mutex.Unlock();
}