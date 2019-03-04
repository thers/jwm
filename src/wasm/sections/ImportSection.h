#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class ImportSection {
    public:
        vec<wasm::importdesc> imports;

        ImportSection(Reader& reader) {
            auto readImport = [&] () {
                wasm::importdesc import;

                import.module = wasm::decoders::name(reader);
                import.nm = wasm::decoders::name(reader);

                auto type = decoders::byteEnumItem<wasm::importtype>(reader);

                switch (type) {
                    case wasm::importtype::it_typeidx:
                        import.x = decoders::u32(reader);
                        break;

                    case wasm::importtype::it_tabletype:
                        import.tt = wasm::decoders::tabletype(reader);
                        break;

                    case wasm::importtype::it_memtype:
                        import.mt = wasm::decoders::memtype(reader);
                        break;

                    case wasm::importtype::it_globaltype:
                        import.gt = wasm::decoders::globaltype(reader);
                        break;
                }

                return import;
            };

            imports = wasm::decoders::vec<wasm::importdesc>(reader, readImport);
        }
    };
}
