#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdio>

#include <stdin.h>
#include <wasm.h>
#include <jit.h>
#include <runtime.h>

using namespace std;

// wasm2wat https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wasm2wat/
// wat2wasm https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wat2wasm/

wasm::content_t readModule(char *path);

int main(int argc, char** argv) {
    auto content = readModule(argv[1]);
    auto module = runtime::decode_module(content);

    printbv(content);

    return 0;
}

wasm::content_t readModule(char *path) {
    ifstream file(path, ios::in|ios::binary);
    file.unsetf(ios::skipws);

    istream_iterator<byte_t> begin(file);
    istream_iterator<byte_t> end;

    return {begin, end};
}
