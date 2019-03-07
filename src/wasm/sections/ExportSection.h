#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

namespace wasm::sections {
    class ExportSection {
    public:
        vec_t<wasm::exportdesc_t> exports;

        explicit ExportSection(Reader& reader) {
            auto readExport = [&] () {
                auto nm = wasm::decoders::name(reader);
                auto type = decoders::byteEnumItem<wasm::exporttype>(reader);
                auto idx = decoders::u32(reader);

                return wasm::exportdesc_t {nm, type, idx};
            };

            exports = wasm::decoders::vec<wasm::exportdesc_t>(reader, readExport);
        }
    };
}
