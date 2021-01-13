/*
 * @Author: your name
 * @Date: 2021-01-12 16:22:53
 * @LastEditTime: 2021-01-13 16:30:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Threadpool_test.cpp
 */
#include "../Threadpool.h"
#include "../ConditionLatch.h"
void print() {
    printf("thread tid = %d\n", CurrentThread::tid());
}

void printi(int i) {
    printf("thread tid = %d, %dth \n", CurrentThread::tid(), i);
}

int main() {
    ConditionLatch latch(1);
    // latch.Wait();

    Threadpool pool(100);
    pool.start(100);
    // Thread in threadpool would execuate task after add task 
    pool.add(print);
    pool.add(print);
    for (int i = 0; i < 100; ++i) {
        pool.add(std::bind(printi, i));
    }
    printf("-----main thread\n");
    // sleep(5);
    pool.add(std::bind(&ConditionLatch::CountDown, &latch));
    pool.stop();
    return 0;
}
