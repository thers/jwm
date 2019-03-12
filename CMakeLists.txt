cmake_minimum_required(VERSION 3.13)
project(jwm)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(jwm
        src/main.cpp
        src/wasm/runtime/Module.cpp)
