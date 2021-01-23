/*
 * @Author: your name
 * @Date: 2021-01-21 10:58:19
 * @LastEditTime: 2021-01-21 15:31:03
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
    return 0;
}