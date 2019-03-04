#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class MemorySection {
    public:
        wasm::vec<wasm::memtype> memories;

        MemorySection(Reader& reader) {
            auto readMemory = [&] () {
                return wasm::decoders::memtype(reader);
            };

            memories = wasm::decoders::vec<wasm::memtype>(reader, readMemory);
        }
    };
}
