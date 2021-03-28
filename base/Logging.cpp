/*
 * @Author: your name
 * @Date: 2021-01-19 17:54:59
 * @LastEditTime: 2021-03-28 20:34:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /WebServer/base/Logging.cpp
 */
#include "Logging.h"

__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
  return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

void defaultOutput(const char* msg, int len)
{
  size_t n = fwrite(msg, 1, len, stdout);
  //FIXME check n
  (void)n;
}

void defaultFlush()
{
  fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
/*
 * TimeStamp class constructor
 */

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
    snprintf(buf, sizeof(buf), "%04d%02d%02d %02d:%02d:%02d.%06ld",
            tm_->tm_year + 1900, tm_->tm_mon + 1, tm_->tm_mday, 
            tm_->tm_hour, tm_->tm_min, tm_->tm_sec,
            microseconds);
    return buf;
}

inline LogStream& operator<<(LogStream& s, const SourceFile& v)
{
  s.append(v.data_, v.size_);
  return s;
}

/*
 * Logger class
 */
const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};
// default logLevel = INFO
Logger::LogLevel initLogLevel()
{
  if (::getenv("MUDUO_LOG_TRACE"))
    return Logger::TRACE;
  else if (::getenv("MUDUO_LOG_DEBUG"))
    return Logger::DEBUG;
  else
  //TODO:default LOG_INFO to LOG_TRACE
    return Logger::TRACE;
}

Logger::LogLevel g_logLevel = initLogLevel();
// LOG_INFO
Logger::Logger(SourceFile file, int line) 
    : time_(TimeStamp::nowStamp()),
      logLevel_(LogLevel::INFO), // defaut level
      filename_(file),
      line_(line),
      stream_()
{
  std::string formatedTime = time_.formatTime();
  stream_ << formatedTime << " " << CurrentThread::tid() << " " << LogLevelName[logLevel_];
}

//LOG_WARN | ERROR | FATAL
Logger::Logger(SourceFile file, int line, LogLevel level)
    : time_(TimeStamp::nowStamp()),
      logLevel_(level),
      filename_(file),
      line_(line),
      stream_()
{
  std::string formatedTime = time_.formatTime();
  stream_ << formatedTime << " " << CurrentThread::tid() << " " << LogLevelName[logLevel_];
}
//LOG_TRACE | LOG_DEBUG

Logger::Logger(SourceFile file, int line, LogLevel level, const char *func)
    : time_(TimeStamp::nowStamp()),
      logLevel_(level),
      filename_(file),
      line_(line),
      stream_()
{
  std::string formatedTime = time_.formatTime();
  stream_ << formatedTime << " " << CurrentThread::tid() << " " << LogLevelName[logLevel_];
}
//LOG_SYSERR | LOG_SYSFATAL
Logger::Logger(SourceFile file, int line, bool toabort)
    : time_(TimeStamp::nowStamp()),
      logLevel_(toabort? LogLevel::FATAL : LogLevel::ERROR),
      filename_(file),
      line_(line),
      stream_()
{
  std::string formatedTime = time_.formatTime();
  stream_ << formatedTime << " " << CurrentThread::tid() << " " << LogLevelName[logLevel_];
  if (toabort) {
    stream_ << strerror_tl(errno) << " (errno=" << errno << ")";
  }
}

// deconstructor function
void Logger::printFileAndLine() {
  stream_ << '-' << filename_ << ':' << line_ << '\n';
}

Logger::~Logger() {
  printFileAndLine();
  const LogStream::Buffer& buf(stream().buffer());
  g_output(buf.data(), buf.size());
  if (logLevel_ == LogLevel::FATAL)  // if logLevel_ == FATAL, then abort()
  {
    g_flush();
    abort();
  }
}
