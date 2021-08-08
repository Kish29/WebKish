#! /bin/bash

curr_branch_name=$(git symbolic-ref --short -q HEAD)

if [ $curr_branch_name != "test/webbench_test" ];then
  echo "Please switch to branch -> test/webbench_test, then run this script"
  exit
fi

if [ ! -d "server_test" ];then
  mkdir server_test
fi
# shellcheck disable=SC2164
cd server_test
cmake ../ -DCMAKE_BUILD_TYPE=Release
make test_httpserver
