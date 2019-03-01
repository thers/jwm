#pragma once

#include <stdin.h>
#include <wasm.h>
#include <parser.h>
#include <decoders.h>

namespace wasm::sections {
    class ImportSection {
    public:
        wasm::size_t size;
        vec<wasm::importdesc> imports;

        ImportSection(wasm::size_t s, Parser& parser) : size(s) {
            auto readImport = [&] () {
                wasm::importdesc import;

                import.module = wasm::decoders::name(parser);
                import.nm = wasm::decoders::name(parser);

                auto type = static_cast<wasm::importtype>(parser.read());

                switch (type) {
                    case wasm::importtype::it_typeidx:
                        import.x = parser.read_u32();
                        break;

                    case wasm::importtype::it_tabletype:
                        import.tt = wasm::decoders::tabletype(parser);
                        break;

                    case wasm::importtype::it_memtype:
                        import.mt = wasm::decoders::memtype(parser);
                        break;

                    case wasm::importtype::it_globaltype:
                        import.gt = wasm::decoders::globaltype(parser);
                        break;
                }

                return import;
            };

            imports = wasm::decoders::vec<wasm::importdesc>(parser, readImport);
        }
    };
}
