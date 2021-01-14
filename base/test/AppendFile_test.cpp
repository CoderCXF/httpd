/*
 * @Author: your name
 * @Date: 2021-01-14 15:56:18
 * @LastEditTime: 2021-01-14 17:00:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/AppendFile_test.cpp
 */
#include "../AppendFile.h"

using namespace std;

int main() {
    AppendFile file("./test.txt");
    const string name("CXF\n");
    const char *firstLine = "My name is CXF\n";
    char *twiceLine = "for two line\n";
    char thirdLine[] = "for third line\n";
    file.append(name, name.size());
    file.append(firstLine, strlen(firstLine));
    cout << "after append first line:" << file.writtenBytes() << endl;
    file.append(twiceLine, strlen(twiceLine));
    file.append(thirdLine, sizeof(thirdLine));
    // file.append();
    file.flush();

    return 0;
}