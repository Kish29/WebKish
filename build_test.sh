#! /bin/bash

mkdir server_test
cmake -DCMAKE_BUILD_TYPE=Release CMakeLists.txt server_test
server_test/test_httpserver
