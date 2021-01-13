/*
 * @Author: your name
 * @Date: 2021-01-11 16:22:43
 * @LastEditTime: 2021-01-12 11:39:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/ConditionLatch_test.cpp
 */
#include "../Thread.h"
#include "../ConditionLatch.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <functional>

using namespace std;

class Test{
public:
    explicit Test(int thread_nums) : conLatch_(1) {
        // can not complete conversion from Thread* to shared_ptr<Thread>
        // std::shared_ptr<Thread> ptr(new Thread(std::bind(&Test::func_, this)));
        // constructor create threads by call func_
        for (int i = 0; i < thread_nums; ++i) {
            threads_.push_back(std::make_shared<Thread>(std::bind(&Test::func_, this)));
        }
        std::for_each(threads_.begin(), threads_.end(), std::bind(&Thread::Start, placeholders::_1));// -1 : every thread object
    }
    // main thread notify all threads start together
    void start() {
        conLatch_.CountDown();
    }
    // join all threads
    void joinAll() {
        std::for_each(threads_.begin(), threads_.end(), std::bind(&Thread::Join, placeholders::_1));
    }
private:
    // callback
    void func_(void) {
        conLatch_.Wait();
        printf("tid = %d\n", CurrentThread::tid());
    }
private:
    ConditionLatch conLatch_;
    std::vector<std::shared_ptr<Thread>> threads_;
};

int main() {
    printf("main thread : pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    Test t(5);
    cout << "sleep before" << endl;
    sleep(5);
    // all threads start together
    t.start();
    t.joinAll();
    printf("progress : pid = %d\n", getpid());
    return 0;
}
