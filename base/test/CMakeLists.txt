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
