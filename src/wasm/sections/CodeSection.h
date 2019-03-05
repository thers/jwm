#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class CodeSection {
    public:
        wasm::vec_t<wasm::code_t> codes;

        explicit CodeSection(Reader& reader) {
            auto readCode = [&] () {
                return wasm::decoders::code(reader);
            };

            codes = wasm::decoders::vec<wasm::code_t>(reader, readCode);
        }
    };
}
