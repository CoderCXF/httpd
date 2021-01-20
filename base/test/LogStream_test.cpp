/*
 * @Author: your name
 * @Date: 2021-01-20 09:36:37
 * @LastEditTime: 2021-01-20 10:08:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/LogStream_test.cpp
 */
#include "../LogStream.h"

using namespace std;

int main() {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    os << 1;
    os << '\n';
    os << 0;
    cout << buf.toString() << endl;

    return 0;
}