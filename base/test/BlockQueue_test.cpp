/*
 * @Author: your name
 * @Date: 2021-01-22 11:42:37
 * @LastEditTime: 2021-01-25 14:04:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/BlockQueue_test.cpp
 */
// FIXME: this test file error: segmentation fault
#include "../BlockQueue.h"
#include "../ConditionLatch.h"
#include "../Thread.h"
#include "../Threadpool.h"

using namespace std;

class Test
{
 public:
  Test(int numThreads)
    : latch_(numThreads)
  {
    for (int i = 0; i < numThreads; ++i)
    {
      char name[32];
      snprintf(name, sizeof name, "work thread %d", i);
      threads_.emplace_back(new Thread(
            std::bind(&Test::threadFunc, this)/*, string(name)*/));
    }
    for (auto& thr : threads_)
    {
      thr->start();
    }
  }

  void run(int times)
  {
    printf("waiting for count down latch\n");
    latch_.Wait(); // all thread wait here
    printf("all threads started\n");
    for (int i = 0; i < times; ++i)
    {
      char buf[64] = {0};
      snprintf(buf, sizeof(buf), "hello %d", i);
      cout << "buf content:" << buf << endl;
      queue_.push(buf); // segmentation fault
      cout << "queue_ has pushed" << endl;
      printf("tid=%d, put data = %s, size = %zd\n", CurrentThread::tid(), buf, queue_.size());
    }
  }

  void joinAll()
  {
    for (size_t i = 0; i < threads_.size(); ++i)
    {
      queue_.push("stop");
    }

    for (auto& thr : threads_)
    {
      thr->join();
    }
  }

 private:

  void threadFunc()
  {
    printf("tid=%d, %s started\n",
           CurrentThread::tid()
           /*CurrentThread::name()*/);

    latch_.CountDown();
    bool running = true;
    while (running)
    {
      std::string d(queue_.take());
      printf("tid=%d, get data = %s, size = %zd\n", CurrentThread::tid(), d.c_str(), queue_.size());
      running = (d != "stop");
    }

    printf("tid=%d, %s stopped\n",
           CurrentThread::tid()
           /*CurrentThread::name()*/);
  }

  BlockQueue<std::string> queue_;
  ConditionLatch latch_;
  std::vector<std::unique_ptr<Thread>> threads_;
};

int main() {
    printf("pid=%d, tid=%d\n", ::getpid(), CurrentThread::tid());
    Test t(5);  // 5 threads
    t.run(100); // main thread to run 100 times
    sleep(10);
    t.joinAll();
    
}
