/*
 * @Author: your name
 * @Date: 2021-01-14 15:56:18
 * @LastEditTime: 2021-01-14 17:41:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/AppendFile_test.cpp
 */
#include "../AppendFile.h"

using namespace std;

int main()
{
	AppendFile write("./test.log");
	
	string str;
	for (int i = 0; i < 26; ++i)
	{
		str.push_back('a' + i);
	}
	
	for (int i = 0; i < 100000; ++i)
	{
		string sId = to_string(i);
		write.append(sId.c_str(), sId.size());
		write.append(" : ", 3);
		write.append(str, str.size());
		write.append("\n", 1);
	}
	
	// write.flush();
}