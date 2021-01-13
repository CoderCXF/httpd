/*
 * @Author: your name
 * @Date: 2021-01-10 15:41:25
 * @LastEditTime: 2021-01-13 09:32:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Singleton_test.cpp
 */
#include "../Singleton.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
class ForTest{
public:
    ForTest() {}
    ~ForTest() {}
    const std::string& name() const { return name_; }
    void setName(const std::string &name) { name_ = const_cast<std::string &>(name); }
private:
    std::string name_;
};
void *threadFunc(void *arg) {
    printf("tid = %lu, %p, name = %s\n",
            pthread_self(), 
            Singleton<ForTest>::GetInstance(), //GetInstance() : return a object
            Singleton<ForTest>::GetInstance().name().c_str()
            );
    Singleton<ForTest>::GetInstance().setName("only one, changed"); // last call
    return NULL;
}
int main() {
    Singleton<ForTest>::GetInstance().setName("only one"); //get instance 
    // child thread
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, threadFunc, NULL);
    if (-1 == ret) {
        fprintf(stderr, "pthread_create error%s\n", strerror(ret));
        return -1;
    }
    sleep(2);  // make sure that child thread execuate firstly
    // main thread
    printf("tid = %lu, %p, name = %s\n",
            pthread_self(), 
            Singleton<ForTest>::GetInstance(), 
            Singleton<ForTest>::GetInstance().name().c_str()
            );
    ret = pthread_join(tid, NULL);
    if (-1 == ret) {
        fprintf(stderr, "pthread_join error%s\n", strerror(ret));
        return -1;
    }
    
    return 0;
}