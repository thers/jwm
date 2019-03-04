#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include <decoders.h>

namespace wasm::sections {
    class ImportSection {
    public:
        vec_t<wasm::importdesc_t> imports;

        explicit ImportSection(Reader& reader) {
            auto readImport = [&] () {
                wasm::importdesc_t import;

                import.module = wasm::decoders::name(reader);
                import.nm = wasm::decoders::name(reader);

                import.type = decoders::byteEnumItem<wasm::importtype>(reader);

                switch (import.type) {
                    case wasm::importtype::it_typeidx:
                        import.x = decoders::u32(reader);
                        break;

                    case wasm::importtype::it_tabletype:
                        import.tt = wasm::decoders::table(reader);
                        break;

                    case wasm::importtype::it_memtype:
                        import.mt = wasm::decoders::mem(reader);
                        break;

                    case wasm::importtype::it_globaltype:
                        import.gt = wasm::decoders::global(reader);
                        break;
                }

                return import;
            };

            imports = wasm::decoders::vec<wasm::importdesc_t>(reader, readImport);
        }
    };
}
