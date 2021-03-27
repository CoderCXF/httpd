set(http_SRCS
    HttpParse.cpp
    HttpServer.cpp
  )

add_library(http ${http_SRCS})
target_link_libraries(http net)

install(TARGETS http DESTINATION lib)
set(HEADERS
  HttpRequest.h
  HttpResponse.h
  HttpParse.h
  HttpServer.h
  )
install(FILES ${HEADERS} DESTINATION include/muduo/net/http)


add_executable(httpserver_test test/HttpServer_test.cpp)
target_link_libraries(httpserver_test http)

