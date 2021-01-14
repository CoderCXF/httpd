/*
 * @Author: your name
 * @Date: 2021-01-13 11:39:06
 * @LastEditTime: 2021-01-14 10:43:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/FixBuffer.h
 */
#include "../FixedBuffer.h"
#include "../Threadpool.h"

using namespace std;

// There can only be one thread at a time access to buffer 
FixedBuffer<4096> buf;
Mutex mutex;
void runInThread() {
    MutexGuard lock(mutex);
    printf("tid = %d\n", CurrentThread::tid());
}

int main() {
    Threadpool pool(5); // task nums = 5;
    pool.start(5);      // thread nums = 5;
    // pool.add(runInThread);

    // pool.stop();
}