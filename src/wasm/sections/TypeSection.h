#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class TypeSection {
    public:
        wasm::vec_t<wasm::functype_t> funcs;

        explicit TypeSection(wasm::Reader& reader) {
            auto readFuncdecl = [&] () {
                return wasm::decoders::functype(reader);
            };

            funcs = wasm::decoders::vec<wasm::functype_t>(reader, readFuncdecl);
        }
    };
}
