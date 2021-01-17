/*
 * @Author: your name
 * @Date: 2021-01-16 15:23:14
 * @LastEditTime: 2021-01-17 11:43:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/AsyncLog_test.cpp
 */
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <malloc.h>
// #include <sys/malloc.h>
#include "../AsyncLog.h"
#include "../Thread.h"

using namespace std;


string str;
AsyncLog asynclog("asynclog_test");

void WriteLog()
{
	// for (int i = 0; i < 100; ++i)
	{
        cout << "test" << endl;
		//std::cout << __FILE__ << " " << __LINE__ << " " << __func__ << " "  << str.size() << std::endl;
		asynclog.append(str.c_str(), str.size());
        cout << "append end" << endl;
	}
}


int main()
{
	asynclog.start();
    sleep(2);
	// for(int n = 1; n < 1000; ++n)
	// {
	// 	for (int i = 0; i < 26; ++i)
	// 	{
	// 		str.push_back('a' + i);
	// 		//asynclog.NotifyLog();
	// 	}
	// }
	// str.push_back('\n');
	
	// int threadNum = 12;
	// std::vector<std::unique_ptr<Thread>> m_threads;
    // std::vector<Thread> m_threads;
	// m_threads.reserve(threadNum);
	// for (int i = 0; i < threadNum; ++i)
	// {
    //     cout << "i = " << i << endl;
	// 	m_threads.push_back(Thread(WriteLog));
	// }
	// sleep(12);
    // std::cout << "m_threads.size:" << m_threads.size() << endl;
    // for (int i = 0; i < threadNum; ++i) {
    //     m_threads[i].Start();
    // }
    // sleep(12);
	// for (int i = 0; i < threadNum; ++i)
	// {
	// 	m_threads[i].Join();
	// }
	
	// asynclog.stop();
    // Thread t(WriteLog);
    // t.Start();
    // sleep(5);
    asynclog.stop();
}