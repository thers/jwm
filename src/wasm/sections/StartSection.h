#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class StartSection {
    public:
        wasm::u32_t funcidx;

        explicit StartSection(Reader& reader)
            : funcidx(wasm::decoders::u32(reader)) {}
    };
}
