/*
 * @Author: your name
 * @Date: 2021-01-19 17:54:59
 * @LastEditTime: 2021-01-22 11:37:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Logging.cpp
 */
#include "Logging.h"

TimeStamp::TimeStamp() : microseconds_(0)
{
    
}

TimeStamp::TimeStamp(int64_t microseconds) : microseconds_(microseconds) {

}

TimeStamp TimeStamp::nowStamp() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t seconds = tv.tv_sec;
    int64_t microseconds = tv.tv_usec;
    return TimeStamp(seconds * kMicrosecondsEachSecond + microseconds);
}

std::string TimeStamp::formatTime() {
    char buf[64];
    time_t nowtime = static_cast<time_t>(microseconds_ / kMicrosecondsEachSecond);
    int64_t microseconds = microseconds_ % kMicrosecondsEachSecond;
    struct tm *tm_;
    tm_ = localtime(&nowtime);
    snprintf(buf, sizeof(buf), "%04d%02d%02d %02d:%02d:%02d.%06d",
            tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday, 
            tm_->tm_hour, tm_->tm_min, tm_->tm_sec,
            microseconds);
    return buf;
}

Logger::Logger(SourceFile file, int line) 
    : time_(TimeStamp::nowStamp()),
      logLevel_(LogLevel::INFO), // defaut level
      filename_(file),
      line_(line),
      stream_()
{

}
Logger::Logger(SourceFile file, int line, LogLevel level)
    : time_(TimeStamp::nowStamp()),
      logLevel_(level),
      filename_(file),
      line_(line),
      stream_()
{

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char *func)
    : time_(TimeStamp::nowStamp()),
      logLevel_(level),
      filename_(file),
      line_(line),
      stream_()
{
  
}

Logger::Logger(SourceFile file, int line, bool flag)
    : time_(TimeStamp::nowStamp()),
      logLevel_(LogLevel::INFO),
      filename_(file),
      line_(line),
      stream_()
{
    
}