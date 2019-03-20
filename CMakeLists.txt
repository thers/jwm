cmake_minimum_required(VERSION 3.13)
project(jwm)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(./include ./src/include ./vendor/xbyak)

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/xbyak)

add_executable(jwm ${SOURCES} src/bin/debug.cpp)

target_link_libraries(jwm fmt)
