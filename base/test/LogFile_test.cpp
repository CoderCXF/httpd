/*
 * @Author: your name
 * @Date: 2021-01-14 20:01:11
 * @LastEditTime: 2021-01-17 16:59:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/LogFile_test.cpp
 */
#include "../LogFile.h"
#include "../Thread.h"
#include "../Threadpool.h"
#include <iostream>
#include <fstream>

using namespace std;

void outputFunc(std::shared_ptr<LogFile> g_file,const char *msg, int len) {
    for (int i = 0; i < 100000; i++) {
        g_file->append(msg, len);
    }
}

void outputFunc(std::shared_ptr<LogFile> g_file, const string& msg, int len) {
    for (int i = 0; i < 100000; i++) {
        g_file->append(msg, len);
    }
}

int main() {
    const string basename("test_log");
    string line = "1234567890 abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ";// 64Bytes
    std::shared_ptr<LogFile> g_file(new LogFile(basename));
    
    Threadpool pool(30);
    pool.start(10);
    pool.add([&g_file, &line](){
        outputFunc(g_file, line, line.size());
    });
    pool.add([&g_file, &line](){
        outputFunc(g_file, line, line.size());
    });
    pool.add([&g_file, &line](){
        outputFunc(g_file, line, line.size());
    });
    pool.add([&g_file, &line](){
        outputFunc(g_file, line, line.size());
    });
    pool.add([&g_file, &line](){
        outputFunc(g_file, line, line.size());
    });
    sleep(5);
    pool.stop();

    return 0;
}