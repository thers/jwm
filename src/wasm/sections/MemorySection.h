#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class MemorySection {
    public:
        wasm::vec_t<wasm::mem_t> memories;

        explicit MemorySection(Reader& reader) {
            auto readMemory = [&] () {
                return wasm::decoders::mem(reader);
            };

            memories = wasm::decoders::vec<wasm::mem_t>(reader, readMemory);
        }
    };
}
