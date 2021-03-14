/*
 * @Author: your name
 * @Date: 2021-01-16 15:23:14
 * @LastEditTime: 2021-03-14 09:17:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/AsyncLog_test.cpp
 */
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
		//std::cout << __FILE__ << " " << __LINE__ << " " << __func__ << " "  << str.size() << std::endl;
		asynclog.append(str.c_str(), str.size());
	}
}


int main()
{
	asynclog.start();
	for(int n = 1; n < 1024; ++n)
	{
		for (int i = 0; i < 26; ++i)
		{
			str.push_back(static_cast<int>(i + 'a'));
			//asynclog.NotifyLog();
		}
	}// 26k
	str.push_back('\n');
    Threadpool pool(10);
	pool.start(10);
	int cntLoop = 0;
	while (cntLoop < 10) {
		pool.add(WriteLog);
		cntLoop++;
	}
	sleep(5);
	pool.stop();
    asynclog.stop();
}