#pragma once

#include <stdin.h>
#include <wasm.h>
#include <parser.h>
#include <decoders.h>

namespace wasm::sections {
    class TypeSection {
    public:
        wasm::size_t size;
        wasm::vec<wasm::funcdecl> funcs;

        TypeSection(wasm::size_t s, Parser& parser) : size(s) {
            auto readFuncdecl = [&] () {
                return wasm::decoders::functype(parser);
            };

            funcs = wasm::decoders::vec<wasm::funcdecl>(parser, readFuncdecl);
        }
    };
}
