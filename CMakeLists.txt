cmake_minimum_required(VERSION 3.13)
project(jwm)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(include)
include_directories(vendor/xbyak/include)

add_subdirectory(vendor/fmt)
add_subdirectory(vendor/xbyak)

add_executable(jwm src/bin/debug.cpp ${SOURCES})

target_link_libraries(jwm fmt)
