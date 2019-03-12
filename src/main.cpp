#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdio>

#include <stdin.h>
#include <wasm.h>
#include <runtime/Module.h>

// jit! https://github.com/herumi/xbyak/tree/master/xbyak
// wasm2wat https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wasm2wat/
// wat2wasm https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wat2wasm/

std::vector<uint8_t> readModule(char *path);

int main(int argc, char** argv) {
    auto moduleContent = readModule(argv[1]);

    wasm::Reader reader(moduleContent);
    runtime::Module module(reader);

    print("\n");
    printbv(moduleContent);

    return 0;
}

std::vector<uint8_t> readModule(char *path) {
    std::ifstream file(path, std::ios::in|std::ios::binary);
    file.unsetf(std::ios::skipws);

    std::istream_iterator<uint8_t> begin(file), end;

    return std::vector<uint8_t> (begin, end);
}
