# Always specify the minimum required version of cmake
cmake_minimum_required(VERSION 3.19)

# set project version
set(
        WEBKISH_VER
        0.0.1
)

# declare a project.
# cmake says it is mandatory and it will define convenient variables
# PROJECT_NAME, PROJECT_VERSION and PROJECT_DESCRIPTION
project(WebKish
        VERSION ${WEBKISH_VER}
        DESCRIPTION "webkish component")

# specify the c++ standard version
set(CMAKE_CXX_STANDARD 11)

# specify release compile flags
set(${CMAKE_CXX_FLAGS_RELEASE} "${CMAKE_CXX_FLAGS} -Wall -O3")

# declare new library
############# libkish ################
set(LIBKISH_NAME kish)

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
        base/list_map.cpp
        base/utils/kish_utils.cpp
        base/file_wrapper.cpp
        net/tcp/wakeup_handler.cpp
        net/tcp/accept_handler.cpp
        net/server_config.cpp
        net/server.h
        net/tcp/tcp_server.cpp
        net/tcp/tcp_handler.cpp
        net/http/http_handler.cpp
        net/http/http_server.cpp
        net/http/http_interface.cpp
        net/http/http_parser.cpp
        net/http/http_heart_check.h
        net/http/http_message.cpp
        net/http/http_request.cpp
        net/http/http_response.cpp
        net/http/http_transform.cpp
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

add_library(${LIBKISH_NAME} SHARED ${SOURCES})
target_link_libraries(${LIBKISH_NAME} pthread)

############ lib mysql #################
set(KSQL_NAME ksql)
set(
        SQL_SRCS
        sql/mysequal/mysql_proxy.cpp
        sql/sql_proxy.h
        sql/mysequal/mysql_conn_pool.cpp
)

add_library(${KSQL_NAME} SHARED ${SQL_SRCS})

target_link_libraries(${KSQL_NAME} ${LIBKISH_NAME} mariadb)

include_directories(sql)
include_directories(sql/mysequal)

############ third party #############
set(THIRD_PARTY_LIBNAME kthird_pt)

set(
        THIRD_PARTY_SRC
        thirdparty/api.c
        thirdparty/http.c
        thirdparty/llhttp.c
        thirdparty/cJSON.c
        thirdparty/CJsonObject.cpp
        thirdparty/multipart_parser.c
)

include_directories(thirdparty)

add_library(${THIRD_PARTY_LIBNAME} SHARED ${THIRD_PARTY_SRC})

# Set VERSION property (optional but it is a good practice)
set_target_properties(
        ${LIBKISH_NAME} ${KSQL_NAME} ${THIRD_PARTY_LIBNAME}
        PROPERTIES
        VERSION ${PROJECT_VERSION}
)


# Declare public API of your library.
# This API will be installed for the third-party application.
# It is a good practice to isolate it in your project tree (like placing it include/ directory).
# Notice that, private headers should not be installed and I strongly suggest to place them with the source files.
set(
        HEADER_LIST
        base/base.h
        base/event_looper.h
        base/fdholder.h
        base/file_wrapper.h
        base/file_writer.h
        base/list_map.h
        base/looper_pool.h
        base/netio_utils.h
        base/sized_buffer.h
        base/sysio_utils.h
        base/time_stamp.h
        base/timer.h
        base/concurr/sync.h
        base/concurr/thread.h
        base/concurr/thread_pool.h
        base/log/logger.h
        base/log/log_stream.h
        base/nio/epoll_handler.h
        base/nio/epoll_poller.h
        base/nio/event_handler.h
        base/nio/poll_poller.h
        base/nio/poller.h
        base/nio/select_poller.h
        base/socket/inet_address.h
        base/socket/socket.h
        base/utils/kish_utils.h
        net/server.h
        net/server_config.h
        net/tcp/accept_handler.h
        net/tcp/tcp_handler.h
        net/tcp/tcp_server.h
        net/tcp/wakeup_handler.h
        net/http/http_handler.h
        net/http/http_heart_check.h
        net/http/http_interface.h
        net/http/http_message.h
        net/http/http_parser.h
        net/http/http_request.h
        net/http/http_response.h
        net/http/http_server.h
        net/http/http_transform.h
        sql/sql_proxy.h
        sql/mysequal/mysql_proxy.h
        sql/mysequal/mysql_conn_pool.h
        thirdparty/cJSON.h
        thirdparty/CJsonObject.hpp
        thirdparty/llhttp.h
        thirdparty/multipart_parser.h
)

set_target_properties(
        ${LIBKISH_NAME} ${KSQL_NAME} ${THIRD_PARTY_LIBNAME}
        PROPERTIES
        PUBLIC_HEADER
        "${HEADER_LIST}"
)

# If you work with subdirectories,
# it is not very convenient to include relative paths like "../include/mylib.h".
# So, pass a top directory in included directories:
target_include_directories(
        ${LIBKISH_NAME}
        PRIVATE .
)
target_include_directories(
        ${KSQL_NAME}
        PRIVATE .
)
target_include_directories(
        ${THIRD_PARTY_LIBNAME}
        PRIVATE .
)
# Create an install rule for your library.
# I suggest to use variables CMAKE_INSTALL_*DIR defined in GNUInstallDirs
include(GNUInstallDirs)

# declare files to install:
install(TARGETS
        ${LIBKISH_NAME} ${KSQL_NAME} ${THIRD_PARTY_LIBNAME}
        LIBRARY
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/kish
        PUBLIC_HEADER
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR}/kish
        )

# You may also export a pkg-config file. This file allows a third-party application to easily import your library:
#
# with Makefile, see https://linux.die.net/man/1/pkg-config
# with Autotools, see https://autotools.io/pkgconfig/pkg_check_modules.html
# with cmake, see https://cmake.org/cmake/help/v3.0/module/FindPkgConfig.html
# Create a template file named webkish.pc.in (see http://manpages.ubuntu.com/manpages/cosmic/man5/pc.5.html for more information):

# ...

# In your CMakeLists.txt, add a rule to expand @ macros
# (@ONLY ask to cmake to not expand variables of the form ${VAR}):
configure_file(webkish.pc.in webkish.pc @ONLY)

# And finally, install generated file:
install(FILES
        ${CMAKE_BINARY_DIR}/webkish.pc
        DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
