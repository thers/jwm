#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class TypeSection {
    public:
        wasm::vec_t<wasm::funcdecl_t> funcs;

        TypeSection(Reader& reader) {
            auto readFuncdecl = [&] () {
                return wasm::decoders::functype(reader);
            };

            funcs = wasm::decoders::vec<wasm::funcdecl_t>(reader, readFuncdecl);
        }
    };
}
