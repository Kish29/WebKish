cmake_minimum_required(VERSION 3.19)
project(WebKish)

set(CMAKE_CXX_STANDARD 11)

set(
        SOURCES
        base/base.h
        base/socket/inet_address.cpp
        base/event_looper.cpp
        base/file_writer.cpp
        base/time_stamp.cpp
        base/sized_buffer.h
        base/log/log_stream.cpp
        base/looper_pool.cpp
        base/sysio_utils.cpp
        base/netio_utils.cpp
        base/socket/socket.cpp
        base/concurr/thread.cpp
        base/concurr/thread_pool.cpp
        base/concurr/sync.h
        base/nio/poller.h
        base/nio/event_handler.h
        base/nio/epoll_poller.cpp
        base/nio/epoll_handler.cpp
        base/nio/select_poller.cpp
        base/nio/poll_poller.cpp
        base/log/logger.cpp
        base/fdholder.cpp
        base/timer.cpp
        base/utils/util.h
        net/tcp/wakeup_handler.cpp
        net/tcp/accept_handler.cpp
        net/server.h
        net/tcp/tcp_server.cpp
        net/tcp/tcp_handler.cpp
        net/http/http_handler.cpp
        net/http/http_server.cpp
        net/http/http_interface.cpp
        net/http/http_parser.cpp
)

include_directories(base)
include_directories(base/nio)
include_directories(base/log)
include_directories(base/socket)
include_directories(base/concurr)
include_directories(base/utils)
include_directories(net)
include_directories(net/tcp)
include_directories(net/http)

set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wall -O3")
set(${CMAKE_CXX_FLAGS_DEBUG} "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -O3")

add_definitions(-D__DEBUG__)

add_library(kbase ${SOURCES})

macro(add_example_target TARGET)
    add_executable(example_${TARGET} example_${TARGET}.cpp)
    target_link_libraries(example_${TARGET} kbase pthread)
endmacro()

macro(add_test_target TARGET)
    add_executable(test_${TARGET} tests/test_${TARGET}.cpp)
    target_link_libraries(test_${TARGET} kbase pthread)
endmacro()

add_test_target(other)
add_test_target(tcpserver)
add_test_target(httpserver)
add_test_target(threadpool)
add_test_target(logger)
add_test_target(timer)
add_test_target(thread)
