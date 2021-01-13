/*
 * @Author: your name
 * @Date: 2021-01-09 15:24:51
 * @LastEditTime: 2021-01-10 14:25:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Mutex_test.cpp
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../Mutex.h"
//global variable,shared source
int g_var = 0;
Mutex mutex;
void* callback1(void *arg) {
    while(true) {
        MutexGuard lock(mutex);
        printf("pthread_id : %lu ",  pthread_self());
        printf("HELLO ");
        sleep(rand() % 3);
        printf("WORLD!\n");
        sleep(rand() % 2);
    }
    return NULL;
}

void* callback2(void *arg) {
    printf("callback1 exit zone and enter callback2\n");

    while(true) {
        printf("callback2 enter while loop before lock\n");
        MutexGuard lock(mutex);
        printf("callback2 enter while loop after lock\n");
        printf("pthread_id : %lu ",  pthread_self());
        printf("hello ");
        sleep(rand() % 3);
        printf("world!\n");
        sleep(rand() % 2); 
    }
    return NULL;
}

int main() {
    int ret1 = 0, ret2 = 0;
    srand(time(NULL));
    // int mode_ = 1;
    // int mode_2 = 0;

    pthread_t tids[2];
    ret1 = pthread_create(&(tids[0]), NULL, callback1, NULL);
    sleep(2);
    ret2 = pthread_create(&(tids[1]), NULL, callback2, NULL);
    sleep(2);
    if (-1 == ret1 || -1 == ret2) {
        perror("pthread_create error");
        return -1;
    }
    printf("main thread execuate here\n");
    if (-1 == pthread_join(tids[0],NULL) || -1 == pthread_join(tids[1], NULL)) {
        perror("pthread_join error");
        return -1;
    }
    return 0;
}
