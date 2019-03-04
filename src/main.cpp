#include <stdin.h>

#include "utils/io.h"
#include "reader.h"
#include "wasm/Module.h"

int main(int argc, char** argv) {
    auto moduleContent = jwmio::readModule(argv[1]);

    Reader reader(moduleContent);
    wasm::Module module;

    module.parse(reader);

    print("\n");
    printbv(moduleContent);

    return 0;
}