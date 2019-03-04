#pragma once

#include <stdin.h>
#include <wasm.h>
#include <parser.h>
#include <decoders.h>

namespace wasm::sections {
    class FunctionSection {
    public:
        wasm::vec<wasm::u32> typeindices;

        FunctionSection(Parser& parser) {
            auto readTypeidx = [&] () {
                return parser.read_u32();
            };

            typeindices = wasm::decoders::vec<wasm::u32>(parser, readTypeidx);
        }
    };
}
