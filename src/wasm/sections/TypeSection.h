#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class TypeSection {
    public:
        wasm::vec_t<wasm::func_t> funcs;

        TypeSection(wasm::Reader& reader) {
            auto readFuncdecl = [&] () {
                return wasm::decoders::functype(reader);
            };

            funcs = wasm::decoders::vec<wasm::func_t>(reader, readFuncdecl);
        }
    };
}
