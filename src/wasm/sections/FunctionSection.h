#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class FunctionSection {
    public:
        wasm::vec_t<wasm::u32_t> typeindices;

        explicit FunctionSection(Reader& reader) {
            auto readTypeidx = [&] () {
                return decoders::u32(reader);
            };

            typeindices = wasm::decoders::vec<wasm::u32_t>(reader, readTypeidx);
        }
    };
}
