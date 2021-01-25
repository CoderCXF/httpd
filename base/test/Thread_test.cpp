/*
 * @Author: your name
 * @Date: 2021-01-11 20:07:15
 * @LastEditTime: 2021-01-25 14:02:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Thread_test.cpp
 */
#include "../Thread.h"

void threadFunc() {
    printf("func tid = %d\n", CurrentThread::tid());
}

void threadFunc2() {
    printf("func2 tid = %d\n", CurrentThread::tid());
}

void threadFunc3() {
    printf("fun3 tid = %d\n", CurrentThread::tid());
}

class Foo{
public:
    explicit Foo(int x) : x_(x){}
    void memberFunc() {
        printf("tid=%d, Foo::x_=%d\n", CurrentThread::tid(), x_);
    }

    void memberFunc2(const std::string& text) {
        printf("tid=%d, Foo::x_=%d, text=%s\n", CurrentThread::tid(), x_, text.c_str());
    }

private:
    int x_;
};

int main() {
    printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    
    Thread t1(threadFunc);
    t1.start();
    t1.join();

    Thread t2(threadFunc2);
    t2.start();
    t2.join();

    Thread t3(threadFunc3);
    t3.start();
    t3.join();

    Foo foo(78);
    Thread t4(std::bind(&Foo::memberFunc, &foo));
    t4.start();
    t4.join();

    Foo foo2(100);
    Thread t5(std::bind(&Foo::memberFunc2, &foo2, std::string("CXF")));
    t5.start();
    t5.join();
    return 0;
}
