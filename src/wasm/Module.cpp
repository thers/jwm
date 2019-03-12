#include "Module.h"

using namespace std;

namespace wasm {
    inline auto readFuncdecl = [] (Reader& reader) {
        return decoders::functype(reader);
    };

    inline auto readImport = [] (Reader& reader) {
        importdesc_t import;

        import.module = decoders::name(reader);
        import.nm = decoders::name(reader);

        import.type = decoders::byteEnumItem<importtype>(reader);

        switch (import.type) {
            case importtype::it_typeidx:
                import.val = decoders::u32(reader);
                break;

            case importtype::it_tabletype:
                import.val = decoders::table(reader);
                break;

            case importtype::it_memtype:
                import.val = decoders::mem(reader);
                break;

            case importtype::it_globaltype:
                import.val = decoders::global(reader);
                break;
        }

        return import;
    };

    inline auto readTypeidx = [] (Reader& reader) {
        return decoders::u32(reader);
    };

    inline auto readTable = [] (Reader& reader) {
        return decoders::table(reader);
    };

    inline auto readMemory = [] (Reader& reader) {
        return decoders::mem(reader);
    };

    inline auto readExport = [] (Reader& reader) {
        auto nm = decoders::name(reader);
        auto type = decoders::byteEnumItem<exporttype>(reader);
        auto idx = decoders::u32(reader);

        return exportdesc_t {nm, type, idx};
    };

    inline auto readCode = [] (Reader& reader) {
        return decoders::code(reader);
    };

    inline auto readElement = [] (Reader& reader) {
        return decoders::element(reader);
    };

    Module::Module(Reader &reader):
            types(),
            imports(),
            functions(),
            tables(),
            memories(),
            exports(),
            codes(),
            start(0),
            elements()
    {
        if (decoders::reinterpretBytes<u32_t>(reader) != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (decoders::reinterpretBytes<u32_t>(reader) != versionNumber) {
            scream("Missing or incompatible version\n");
        }

        print("Module is valid, reading sections:\n");

        while (!reader.eof()) {
            auto [type, size] = decoders::section(reader);
            auto pos_before = reader.get_pos();

            print("\t%s, size %d, ", wasm::section_names[type], size);

            switch (type) {
                case wasm::section::s_type:
                    decoders::vec2<module_types_t>(&types, reader, readFuncdecl);
                    break;

                case wasm::section::s_import:
                    decoders::vec2<module_imports_t>(&imports, reader, readImport);
                    break;

                case wasm::section::s_function:
                    decoders::vec2<module_functions_t>(&functions, reader, readTypeidx);
                    break;

                case wasm::section::s_table:
                    decoders::vec2<module_tables_t>(&tables, reader, readTable);
                    break;

                case wasm::section::s_memory:
                    decoders::vec2<module_memories_t>(&memories, reader, readMemory);
                    break;

                case wasm::section::s_export:
                    decoders::vec2<module_exports_t>(&exports, reader, readExport);
                    break;

                case wasm::section::s_code:
                    decoders::vec2<module_codes_t>(&codes, reader, readCode);
                    break;

                case wasm::section::s_start:
                    start = decoders::u32(reader);
                    break;

                case wasm::section::s_element:
                    decoders::vec2<module_element_t>(&elements, reader, readElement);
                    break;
            }

            print("done.\n");

            reader.seek_to(pos_before + size);
        }
    }
}
