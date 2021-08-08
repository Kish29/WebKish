#! /bin/bash

if [ ! -d "server_test"];then
  mkdir server_test
fi
cd server_test
cmake ../ -DCMAKE_BUILD_TYPE=Release
make test_httpserver
