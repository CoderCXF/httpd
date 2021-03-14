# # Mutex
# add_executable(Mutex Mutex.cpp)
# target_link_libraries(Mutex pthread base)

# # Condition
# add_executable(Condition Mutex.cpp Thread.cpp Condition.cpp test/Condition_test.cpp)
# target_link_libraries(Condition pthread base)

# # Thread
# add_executable(Thread Mutex.cpp Thread.cpp test/Thread_test.cpp)
# target_link_libraries(Thread pthread base)

# # Threadpool
# add_executable(Threadpool Mutex.cpp Thread.cpp Condition.cpp ConditionLatch.cpp Threadpool.cpp test/Threadpool_test.cpp)
# target_link_libraries(Threadpool pthread base)

# # Singleton
# add_executable(Singleton test/Singleton_test.cpp)
# target_link_libraries(Singleton pthread base)

# # AppendFile
# add_executable(AppendFile AppendFile.cpp test/AppendFile_test.cpp)
# target_link_libraries(AppendFile pthread base)

# # LogFile
# add_executable(LogFile LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/LogFile_test.cpp)
# target_link_libraries(LogFile pthread base)

# # FixedBuffer
# add_executable(FixedBuffer Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp test/FixedBuffer_test.cpp)
# target_link_libraries(FixedBuffer pthread base)

# # AsyncLog
# add_executable(AsyncLog AsyncLog.cpp LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/AsyncLog_test.cpp)
# target_link_libraries(AsyncLog pthread base)

# #LogStream
# #[[
# add_executable(LogStream Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp test/LogStream_test.cpp)
# target_link_libraries(LogStream pthread)
# ]]

# # Logging
# add_executable(Logging Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp Logging.cpp test/Logging_test.cpp)
# target_link_libraries(Logging pthread base)

# # BlockQueue
# add_executable(BlockQueue Mutex.cpp Condition.cpp  ConditionLatch.cpp Thread.cpp Threadpool.cpp test/BlockQueue_test.cpp)
# target_link_libraries(BlockQueue pthread base)

# # Timestamp
# add_executable(Timestamp Timestamp.cc test/Timestamp_test.cpp)
# target_link_libraries(Timestamp pthread base)

# Mutex
add_executable(Mutex Mutex_test.cpp)
target_link_libraries(Mutex pthread base)

# Condition
add_executable(Condition Condition_test.cpp)
target_link_libraries(Condition pthread base)

# Thread
add_executable(Thread Thread_test.cpp)
target_link_libraries(Thread pthread base)

# Threadpool
add_executable(Threadpool Threadpool_test.cpp)
target_link_libraries(Threadpool pthread base)

# Singleton
add_executable(Singleton Singleton_test.cpp)
target_link_libraries(Singleton pthread base)

# AppendFile
add_executable(AppendFile AppendFile_test.cpp)
target_link_libraries(AppendFile pthread base)

# LogFile
add_executable(LogFile LogFile_test.cpp)
target_link_libraries(LogFile pthread base)

# FixedBuffer
add_executable(FixedBuffer FixedBuffer_test.cpp)
target_link_libraries(FixedBuffer pthread base)

# AsyncLog
# add_executable(AsyncLog AsyncLog_test.cpp)
# target_link_libraries(AsyncLog pthread base)

#LogStream
#[[
add_executable(LogStream Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp test/LogStream_test.cpp)
target_link_libraries(LogStream pthread)
]]

# Logging
add_executable(Logging Logging_test.cpp)
target_link_libraries(Logging pthread base)

# BlockQueue
add_executable(BlockQueue BlockQueue_test.cpp)
target_link_libraries(BlockQueue pthread base)

# Timestamp
add_executable(Timestamp Timestamp_test.cpp)
target_link_libraries(Timestamp pthread base)
