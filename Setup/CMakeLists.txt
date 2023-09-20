cmake_minimum_required(VERSION 3.26)
project(Setup)

set(CMAKE_CXX_STANDARD 17)

add_executable(Setup main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(Setup PRIVATE box2d::box2d)