/*
 * @Author: your name
 * @Date: 2021-03-25 20:18:09
 * @LastEditTime: 2021-03-26 20:50:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/function.cpp
 */
#include <iostream>
#include <stdlib.h>
#include <functional>

typedef std::function<void()> Functor1;
typedef std::function<void(int)> Functor2;
void callback(Functor1 cb) {
    cb();
}

void fun2(int a) {
    printf("a = %d\n", a);
}

int main() {
    char buf[32];
    Functor2 functor2(fun2);
    callback(std::bind(functor2, 2));
    return 0;
}