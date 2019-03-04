#include <stdin.h>
#include <wasm.h>

#include "utils/io.h"
#include "wasm/Module.h"

int main(int argc, char** argv) {
    auto moduleContent = jwmio::readModule(argv[1]);

    wasm::Reader reader(moduleContent);
    wasm::Module module;

    module.parse(reader);

    print("\n");
    printbv(moduleContent);

    return 0;
}