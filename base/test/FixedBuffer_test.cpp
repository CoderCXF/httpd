/*
 * @Author: your name
 * @Date: 2021-01-13 11:39:06
 * @LastEditTime: 2021-01-19 15:10:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/test/FixBuffer.h
 */
#include "../FixedBuffer.h"
#include "../Threadpool.h"
#include "../LogFile.h"
#include <memory>

using namespace std;

const int kFixedBufferSize = 4096;
typedef FixedBuffer<kFixedBufferSize> Buffer;

void printBuffer(Buffer &buf) {
    int cnt = 0;
    for (int i = 0; i < buf.size(); ++i) {
        std::cout << buf[i] << " ";
        cnt++;
        if (!(cnt % 7)) {
            printf("\n");
        }
    }
    printf("\n");
}

int main() {
    FixedBuffer<kFixedBufferSize> buf;
    srand(time(NULL));
    char alpha[27];
    char ch = 'a';
    for (int i = 0; i < 26; i++) {
        alpha[i] = ch++;
    }
    alpha[sizeof(alpha) - 1] = '\0';
    
    // for (int i = 0; i < sizeof(alpha); ++i) {
    //     cout << alpha[i] << endl;
    // }
    
    // append
    buf.append(alpha, sizeof alpha);
    printBuffer(buf);

    // size()
    cout << "buf.size: " << buf.size() << endl;
    
    // avail()
    cout << "buf.avail: " << buf.avail() << endl;

    // data()
    cout << "buf.data: " << buf.data() << endl;

    // zero
    buf.bzero();
    printBuffer(buf);
    cout << "buf.size: " << buf.size() << endl;

    // add
    buf.add(1);
    cout << "buf.size: " << buf.size() << endl;

    //reset
    buf.reset();
    cout << "buf.size: " << buf.size() << endl;

    return 0;
}