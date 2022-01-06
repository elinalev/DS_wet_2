cmake_minimum_required(VERSION 2.8.12.2)
project(DS_wet_2)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

add_executable(DS_wet_2 test_hash_table.cpp hash_table.h utils.cpp)
