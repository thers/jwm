cmake_minimum_required(VERSION 3.13)
project(jwm)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)

add_executable(jwm
        src/main.cpp
        src/utils/io.h
        src/utils/io.cpp
        src/include/wasm/opcodes.h
        src/include/wasm/types.h
        src/include/stdin.h
        src/include/wasm/sections.h
        src/include/wasm.h
        src/include/decoders.h
        src/wasm/Module.cpp
        src/wasm/Module.h
        function.cpp
        src/wasm/sections/TypeSection.h
        src/wasm/sections/ImportSection.h
        src/wasm/sections/FunctionSection.h
        src/wasm/sections/TableSection.h
        src/wasm/sections/MemorySection.h
        )
