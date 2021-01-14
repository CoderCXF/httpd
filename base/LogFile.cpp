/*
 * @Author: your name
 * @Date: 2021-01-14 11:47:23
 * @LastEditTime: 2021-01-14 15:15:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/LogFile.cpp
 */
#include "LogFile.h"

std::string Helper::hostname() {
    char name[256];
    if (0 == gethostname(name, sizeof(name))) {
        return std::string(name);
    } else {
        return "unknown host";
    }
}

LogFile::LogFile(const std::string& basename) 
    : basename_(basename),
      count_(0),
      mutex_(new Mutex()),
      startOfPeriod_(0),
      lastRoll_(0),
      lastFlush_(0) 
{ 

}

LogFile::~LogFile() {
    
}

void LogFile::append(const std::string& logline, size_t len) {
    std::string fileName = getLogFileName(basename_);
    FILE* fd = fopen(fileName.c_str(), "ae");
    
}

std::string LogFile::getLogFileName(const std::string& basename) {
    std::string fileName(basename);
    time_t now;
    struct tm *local_tm;
    time(&now);
    local_tm = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local_tm);
    fileName += buf;
    fileName += Helper::hostname();
    char pidbuf[32];
    snprintf(pidbuf, sizeof(pidbuf), ".%d", getpid());
    fileName += getpid();
    fileName += ".log";

    return fileName;
}