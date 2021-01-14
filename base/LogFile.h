/*
 * @Author: your name
 * @Date: 2021-01-14 11:47:11
 * @LastEditTime: 2021-01-14 15:09:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/LogFile.h
 */
#include "Base.h"
#include "pthread.h"
#include <time.h>
#include "Mutex.h"
#include <string>
#include <memory>

class Helper{
public:
    static std::string hostname();
    // static 
};

class LogFile{
public:
    explicit LogFile(const std::string& basename);
    ~LogFile();
    void append(const std::string& basename, size_t len);
    void flush();
    void rollFile();

private:
    static std::string getLogFileName(const std::string& basename);
    const std::string basename_;
    
    int count_;

    std::unique_ptr<Mutex> mutex_;
    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;

    const static int rollSize_ = 64 * 1024;
    const static int flushInterval_ = 3;
    const static int kRollPerSeconds_ = 60 * 60 * 24;
};