/*
 * @Author: your name
 * @Date: 2021-01-10 15:05:01
 * @LastEditTime: 2021-01-13 09:50:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Singleton.h
 */
#ifndef _SINGLETON_H_
#define _SINGLETON_H_
#include <pthread.h>
#include <iostream>
#include <memory>
/*pthread_once is the most efficient*/
template <typename T>
class Singleton{
public:
    Singleton() = delete;
    ~Singleton() = delete;
    static T & GetInstance() { // Interface
        pthread_once(&once_ctl, &init); // only one time
        return (*(ptrInstance.get()));
    }
private:
    static void init() {
        ptrInstance = std::unique_ptr<T>(new T());
    }

private:
    static pthread_once_t once_ctl;
    static std::unique_ptr<T> ptrInstance;
};
// static member variable initilizer
template <typename T>
pthread_once_t Singleton<T>::once_ctl = PTHREAD_ONCE_INIT;

template <typename T>
std::unique_ptr<T> Singleton<T>::ptrInstance;

// 2. DCL
/*
class Singleton{
public:
    static Singleton* getInstance() {
        if (instance == NULL) { //first check
            pthread_mutex_lock(&mutex);
            if (instance == NULL) { //second check
                instance = new Singleton();
            }
            pthread_mutex_unlock(&mutex);
        }
        return instance;
    }
private:
    Singleton();
private:
    static pthread_mutex_t mutex;
    static Singleton* instance;
};
Singleton* Singleton::instance = NULL;
pthread_mutex_t Singleton::mutex;

*/
// 3. thread safety itself
/*
class Signleton{
public:
    static Signleton* getInstance() {
        return instance;       
    }
private:
    static Singleton *instance;
};
Singleton* Singleton::instance = new Singleton();
*/
#endif
