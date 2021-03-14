set(base_SRCS
  Mutex.cpp
  Condition.cpp
  ConditionLatch.cpp
  LogFile.cpp
  Logging.cpp
  LogStream.cpp
  Thread.cpp
  Threadpool.cpp
  Timestamp.cpp
  AppendFile.cpp
  )

add_library(base ${base_SRCS})
target_link_libraries(base pthread rt)


install(TARGETS base DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/muduo/base)

# if(BUILD_EXAMPLES)
  add_subdirectory(test)
# endif()
