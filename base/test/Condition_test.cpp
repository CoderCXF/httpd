/*
 * @Author: your name
 * @Date: 2021-01-05 10:33:50
 * @LastEditTime: 2021-03-14 09:23:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /Linux_code/multi_thread/pc.c
 */
#include "../Condition.h"
#include "string.h"

struct Tickets
{
    int tick_num;
} tick;

int t;
Mutex mutex;
Condition cond(mutex);
//producer thread
void *producer(void *arg) {
    while (1) {
        // pthread_mutex_lock(&mutex);
        MutexGuard lock(mutex);
        t = tick.tick_num++;
        printf("Producer thread: original num = %d, tick_num = %d\n", t, tick.tick_num);
        // pthread_mutex_unlock(&mutex);
        // pthread_cond_signal(&cond); //通知阻塞在该变量的线程
        cond.Notify();
        sleep(rand() % 5);
    }
    return NULL;
}
//customer thread
void *customer(void *arg) {
    while (1) {
        // pthread_mutex_lock(&mutex);
        MutexGuard lock(mutex);
        while (tick.tick_num == 0) {
            // pthread_cond_wait(&cond, &mutex); //消费者阻塞在条件变量上
            cond.Wait();
        }
        t = tick.tick_num--;
        printf("Customer thread: original num = %d, tick_num = %d\n", t, tick.tick_num);
        // pthread_mutex_unlock(&mutex);
        sleep(rand() % 5);
    }
    return NULL;
}
int main() {
    int ret;
    pthread_t tid_pro, tid_customer;
    srand((unsigned)time(0));
    tick.tick_num = 0;
    ret = pthread_create(&tid_pro, NULL, producer, NULL);
    if (ret != 0) {
        fprintf(stderr, "pthread_create error%s\n", strerror(ret));
        exit(-1);
    }
    ret = pthread_create(&tid_customer, NULL, customer, NULL);
    if (ret != 0) {
        fprintf(stderr, "pthread_create error%s\n", strerror(ret));
        exit(-1);
    }
    pthread_join(tid_pro, NULL);
    pthread_join(tid_customer, NULL);
    return 0;
}

