#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <cstdio>

#include <jwm/jwm.h>

using namespace std;
using namespace jwm;

// wasm2wat https://cdn.rawgit.com/WebAssembly/wabt/master/demo/wasm2wat/
// wat2wasm https://cdn.rawgit.com/WebAssembly/wabt/master/demo/wat2wasm/

wasm::content_t readModule(char *path);

int main(int argc, char** argv) {
    auto content = readModule(argv[1]);
    ModuleImports imports {
        {"test", {
            {"int", 123}
        }}
    };

    ModuleInstance module(content, imports);

    args_t args = {20, 2726347};
    auto ret = module.start(args);

    print("Retval: {}", std::get<i32_t>(ret));

    return 0;
}

wasm::content_t readModule(char *path) {
    ifstream file(path, ios::in|ios::binary);
    file.unsetf(ios::skipws);

    istream_iterator<byte_t> begin(file);
    istream_iterator<byte_t> end;

    return {begin, end};
}
