#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdio>

#include <stdin.h>
#include <wasm.h>
#include <runtime/Module.h>

using namespace std;

// jit! https://github.com/herumi/xbyak/tree/master/xbyak
// wasm2wat https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wasm2wat/
// wat2wasm https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wat2wasm/

wasm::content_t readModule(char *path);

int main(int argc, char** argv) {
    auto moduleContent = readModule(argv[1]);

    wasm::Reader reader(moduleContent);
    runtime::Module module(reader);

    printbv(moduleContent);

    return 0;
}

wasm::content_t readModule(char *path) {
    ifstream file(path, ios::in|ios::binary);
    file.unsetf(ios::skipws);

    istream_iterator<byte_t> begin(file);
    istream_iterator<byte_t> end;

    return {begin, end};
}
