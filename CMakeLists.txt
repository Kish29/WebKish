cmake_minimum_required(VERSION 3.19)
project(WebKish)

set(CMAKE_CXX_STANDARD 11)

set(
        SOURCES
        base/base.h
        base/inet_address.cpp
        base/event_looper.cpp
        base/file_writer.cpp
        base/time_stamp.cpp
        base/sized_buffer.h
        base/log_stream.cpp
        base/looper_pool.cpp
        base/sysio_utils.cpp
        base/netio_utils.cpp
        base/socket.cpp
        base/thread.cpp
        base/sync.h
        base/nio/poller.h
        base/nio/event_handler.h
        base/nio/epoll_poller.cpp
        base/nio/epoll_handler.cpp
        base/nio/select_poller.cpp
        base/nio/poll_poller.cpp
        base/logger.cpp
        base/fdholder.cpp
        base/timer.cpp
        net/wakeup_handler.cpp
        net/handler/accept_handler.cpp
        net/server.h
        net/tcp_server.cpp
        net/handler/tcp_handler.cpp
        net/handler/http_handler.cpp
        net/http_server.cpp
        net/http_interface.cpp
        net/http_parser.cpp
)

include_directories(base)
include_directories(base/nio)
include_directories(net)
include_directories(net/handler)

set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -Wall -O3")
set(${CMAKE_CXX_FLAGS_DEBUG} "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")

add_definitions(-D__DEBUG__)

add_library(kbase ${SOURCES})

macro(add_example_target TARGET)
    add_executable(example_${TARGET} example_${TARGET}.cpp)
    target_link_libraries(example_${TARGET} kbase pthread)
endmacro()

add_example_target(tcpserver)
add_example_target(httpserver)
add_example_target(main)
add_example_target(loggertest)
