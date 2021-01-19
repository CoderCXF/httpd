/*
 * @Author: your name
 * @Date: 2021-01-16 15:23:14
 * @LastEditTime: 2021-01-19 15:27:51
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
#include "../Threadpool.h"

using namespace std;


string str;
AsyncLog asynclog("asynclog_test");

void WriteLog()
{
	for (int i = 0; i < 10000; ++i)
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
	for(int n = 1; n < 1024; ++n)
	{
		for (int i = 0; i < 26; ++i)
		{
			str.push_back('a' + i);
			//asynclog.NotifyLog();
		}
	}
	str.push_back('\n');
    
    asynclog.stop();
}