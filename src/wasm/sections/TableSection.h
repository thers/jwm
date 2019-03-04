#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class TableSection {
    public:
        wasm::vec_t<wasm::table_t> tables;

        explicit TableSection(Reader& reader) {
            auto readTable = [&] () {
                return wasm::decoders::table(reader);
            };

            tables = wasm::decoders::vec<wasm::table_t>(reader, readTable);
        }
    };
}
