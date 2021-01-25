/*
 * @Author: your name
 * @Date: 2021-01-21 10:58:19
 * @LastEditTime: 2021-01-25 13:47:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Logging_test.cpp
 */
#include "../Logging.h"
#include "../Threadpool.h"

using namespace std;

void logInThread() {
    LOG_INFO << "logInThread";
    usleep(1000);
}

int main() {
    /* Test TimeStamp */
    cout << "-------------------------" << endl;
    TimeStamp now(TimeStamp::nowStamp());
    cout << now.formatTime() << endl;
    cout << "-------------------------" << endl;
    
    /* Test basic LOG_* */
    LOG_TRACE << "trace";
    LOG_DEBUG << "debug";
    LOG_INFO << "Hello";
    LOG_WARN << "World";
    LOG_ERROR << "Error";
    //LOG_FATAL << "Fatal";  // logLevel = LOG_FATAL, will abort

    /* Test Threadpool LOG_* */
    Threadpool pool(10);
    pool.start(8);
    pool.add(logInThread);
    pool.add(logInThread);
    pool.add(logInThread);
    pool.add(logInThread);
    pool.add(logInThread);
    sleep(1);
    pool.stop();
    
    return 0;
}