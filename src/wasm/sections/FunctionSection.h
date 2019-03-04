#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class FunctionSection {
    public:
        wasm::vec<wasm::u32> typeindices;

        FunctionSection(Reader& reader) {
            auto readTypeidx = [&] () {
                return decoders::u32(reader);
            };

            typeindices = wasm::decoders::vec<wasm::u32>(reader, readTypeidx);
        }
    };
}
