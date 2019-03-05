#include <stdin.h>
#include <wasm.h>

#include "utils/io.h"
#include "wasm/Module.h"

// jit! https://github.com/herumi/xbyak/tree/master/xbyak
// wasm2wat https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wasm2wat/
// wat2wasm https://cdn.rawgit.com/WebAssembly/wabt/fb986fbd/demo/wat2wasm/

int main(int argc, char** argv) {
    auto moduleContent = jwmio::readModule(argv[1]);

    wasm::Reader reader(moduleContent);
    wasm::Module module;

    print("\n");
    printbv(moduleContent);

    module.parse(reader);

    return 0;
}