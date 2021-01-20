cmake_minimum_required(VERSION 3.19.0)
project(Server C CXX)

set (INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src
                    /usr/local/include
					)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib
            /usr/local/lib
	    /usr/lib/x86_64-linux-gnu)

add_compile_options(-std=c++11 -g -Wall)

include_directories(${INCLUDE_DIR})
link_directories(${LIB_DIR})
##################################################

include(CTest)
enable_testing()
add_test(NAME LogStream COMMAND LogStream)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
 
include_directories(${Boost_INCLUDE_DIRS})
 
add_executable(LogStream Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp test/LogStream_test.cpp)
 
target_link_libraries(LogStream ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pthread)

###################################################
# EXECUTABLE_OUTPUT_PATH
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# LIBRARY_OUTPUT_PATH
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#set (BASE_SRC
#     Mutex.cpp
#     Condition.cpp
#     ConditionLatch.cpp
#     Thread.cpp
#     Threadpool.cpp)


#add_library(server_base ${BASE_SRC})
#target_link_libraries(server_base pThread)

# Mutex
add_executable(Mutex Mutex.cpp test/Mutex_test.cpp)
target_link_libraries(Mutex pthread)

# Condition
add_executable(Condition Mutex.cpp Thread.cpp Condition.cpp test/Condition_test.cpp)
target_link_libraries(Condition pthread)

# Thread
add_executable(Thread Mutex.cpp Thread.cpp test/Thread_test.cpp)
target_link_libraries(Thread pthread)

# Threadpool
add_executable(Threadpool Mutex.cpp Thread.cpp Condition.cpp ConditionLatch.cpp Threadpool.cpp test/Threadpool_test.cpp)
target_link_libraries(Threadpool pthread)

# Singleton
add_executable(Singleton test/Singleton_test.cpp)
target_link_libraries(Singleton pthread)

# AppendFile
add_executable(AppendFile AppendFile.cpp test/AppendFile_test.cpp)
target_link_libraries(AppendFile pthread)

# LogFile
add_executable(LogFile LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/LogFile_test.cpp)
target_link_libraries(LogFile pthread)

# FixedBuffer
add_executable(FixedBuffer Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp test/FixedBuffer_test.cpp)
target_link_libraries(FixedBuffer pthread)

# AsyncLog
add_executable(AsyncLog AsyncLog.cpp LogFile.cpp Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp test/AsyncLog_test.cpp)
target_link_libraries(AsyncLog pthread)

#LogStream
#[[
add_executable(LogStream Mutex.cpp Condition.cpp Thread.cpp Threadpool.cpp AppendFile.cpp LogFile.cpp LogStream.cpp test/LogStream_test.cpp)
target_link_libraries(LogStream pthread)
]]

