#include <stdin.h>

#include "utils/io.h"
#include "parser.h"
#include "wasm/Module.h"

int main(int argc, char ** argv) {
    auto moduleContent = jwmio::readModule(argv[1]);

    Parser parser(moduleContent);
    wasm::Module module;

    module.parse(parser);

    print("\n");
    printbv(moduleContent);

    return 0;
}