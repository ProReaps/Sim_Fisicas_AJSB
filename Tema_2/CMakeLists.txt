cmake_minimum_required(VERSION 3.26)
project(Tema_2)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tema_2 main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(Tema_2 PRIVATE box2d::box2d)
