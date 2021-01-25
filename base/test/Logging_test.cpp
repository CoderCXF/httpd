/*
 * @Author: your name
 * @Date: 2021-01-21 10:58:19
 * @LastEditTime: 2021-01-24 11:37:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/Logging_test.cpp
 */
#include "../Logging.h"

using namespace std;

int main() {
    cout << "-------------------------" << endl;
    TimeStamp now(TimeStamp::nowStamp());
    cout << now.formatTime() << endl;
    cout << "-------------------------" << endl;
    // cout << __FILE__ << __func__ << __LINE__ << endl;
    LOG_TRACE << "trace";
    LOG_DEBUG << "debug";
    LOG_INFO << "Hello";
    LOG_WARN << "World";
    LOG_ERROR << "Error";
    return 0;
}