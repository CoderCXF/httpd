/*
 * @Author: your name
 * @Date: 2021-01-19 17:54:51
 * @LastEditTime: 2021-01-20 16:33:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/logging.h
 */
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "LogStream.h"
class TimeForm{
public:
    static TimeForm formTime();
private:
    time_t tm;
};

class SourceFile{

};

class Logger{
public:
    enum LogLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };
    Logger();
    ~Logger();
    LogStream& stream() {
        return stream_;
    }
private:
    TimeForm time_;
    LogLevel logLevel_;
    LogStream stream_;
    SourceFile file_;
};

#define LOG_TRACE if (Logger::logLevel() <= ::Logger::TRACE) \
  ::Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (::Logger::logLevel() <= ::Logger::DEBUG) \
  ::Logger(__FILE__, __LINE__, ::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (::Logger::logLevel() <= ::Logger::INFO) \
  ::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN ::Logger(__FILE__, __LINE__, ::Logger::WARN).stream()
#define LOG_ERROR ::Logger(__FILE__, __LINE__, ::Logger::ERROR).stream()
#define LOG_FATAL ::Logger(__FILE__, __LINE__, ::Logger::FATAL).stream()
#define LOG_SYSERR ::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL ::Logger(__FILE__, __LINE__, true).stream()

#endif