cmake_minimum_required(VERSION 3.26)
project(Proyecto_Parcial2)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.5.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(Proyecto_Parcial2 main.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries(Proyecto_Parcial2 PRIVATE box2d::box2d sfml-system sfml-network sfml-graphics sfml-window)
