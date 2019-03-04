#pragma once

#include <stdin.h>
#include <wasm.h>
#include <parser.h>
#include <decoders.h>

namespace wasm::sections {
    class TypeSection {
    public:
        wasm::vec<wasm::funcdecl> funcs;

        TypeSection(Parser& parser) {
            auto readFuncdecl = [&] () {
                return wasm::decoders::functype(parser);
            };

            funcs = wasm::decoders::vec<wasm::funcdecl>(parser, readFuncdecl);
        }
    };
}
