#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class TableSection {
    public:
        wasm::vec<wasm::tabletype> tables;

        TableSection(Reader& reader) {
            auto readTable = [&] () {
                return wasm::decoders::tabletype(reader);
            };

            tables = wasm::decoders::vec<wasm::tabletype>(reader, readTable);
        }
    };
}
