/*
 * @Author: your name
 * @Date: 2021-01-19 17:54:51
 * @LastEditTime: 2021-03-16 11:44:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/logging.h
 */
#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "LogStream.h"
#include "Thread.h"
// #include "TimeStamp.h"
#include <sys/time.h>
#include <time.h>

class TimeStamp{
public:
    TimeStamp();
    explicit TimeStamp(int64_t microseconds);
    ~TimeStamp() = default;
    static TimeStamp nowStamp();
    std::string formatTime(); 
private:
    int64_t microseconds_;
    static const int kMicrosecondsEachSecond = 1000 * 1000;
};

class SourceFile{
    public:
    template<int N>
    SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1)
    {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash)
      {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }
    // import
    explicit SourceFile(const char* filename)
      : data_(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
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
    // Logger() = default;
    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char *func);
    Logger(SourceFile file, int line, bool toabort);
    ~Logger();
    
    LogStream& stream() { return stream_; }
    static LogLevel logLevel();

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();

private:
    void printFileAndLine();
    
    TimeStamp time_;
    LogLevel logLevel_;
    SourceFile filename_;
    int line_;
    LogStream stream_;
};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}

#define LOG_TRACE if (Logger::logLevel() <= ::Logger::TRACE) \
  ::Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= ::Logger::DEBUG) \
  ::Logger(__FILE__, __LINE__, ::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= ::Logger::INFO) \
  ::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN ::Logger(__FILE__, __LINE__, ::Logger::WARN).stream()
#define LOG_ERROR ::Logger(__FILE__, __LINE__, ::Logger::ERROR).stream()
#define LOG_FATAL ::Logger(__FILE__, __LINE__, ::Logger::FATAL).stream()
#define LOG_SYSERR ::Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL ::Logger(__FILE__, __LINE__, true).stream()

#endif