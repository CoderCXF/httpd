include(CheckFunctionExists)

check_function_exists(accept4 HAVE_ACCEPT4)
if(NOT HAVE_ACCEPT4)
  set_source_files_properties(SocketsOps.cc PROPERTIES COMPILE_FLAGS "-DNO_ACCEPT4")
endif()

set(net_SRCS
  EventLoop.cpp
  EventLoopThread.cpp
  EventLoopThreadPool.cpp
  Channel.cpp
  EPoll.cpp
  SocketOps.cpp
  Socket.cpp
  AddrStruct.cpp
  Acceptor.cpp
  TcpServer.cpp
  Buffer.cpp
  Connection.cpp
  )


add_library(net ${net_SRCS})
target_link_libraries(net base)

#add_library(muduo_net_cpp11 ${net_SRCS})
#target_link_libraries(muduo_net_cpp11 muduo_base_cpp11)
#set_target_properties(muduo_net_cpp11 PROPERTIES COMPILE_FLAGS "-std=c++0x")

install(TARGETS net DESTINATION lib)
#install(TARGETS muduo_net_cpp11 DESTINATION lib)

set(HEADERS
  EventLoop.h
  EventLoopThread.h
  EventLoopThreadPool.h
  Channel.h
  EPoll.h
  SocketOps.h
  Socket.h
  AddrStruct.h
  Acceptor.h
  TcpServer.h
  Buffer.h
  Connection.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net)

add_subdirectory(httpd)
#add_subdirectory(inspect)

#TODO:
#if(MUDUO_BUILD_EXAMPLES)
 add_subdirectory(test)
#endif()

#if(PROTOBUF_FOUND)
#  add_subdirectory(protobuf)
#  add_subdirectory(protorpc)
#else()
#  add_subdirectory(protobuf EXCLUDE_FROM_ALL)
#  add_subdirectory(protorpc EXCLUDE_FROM_ALL)
#endif()
