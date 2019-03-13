#include <runtime/Module.h>

namespace runtime {
    Module decode_module(content_t &content) {
        Reader reader(content);

        return Module {reader};
    }

    void parse_module(std::string& content) {
        scream("Text format is not supported, use dedicated utilities to compile binary");
    }

    Module::Module(Reader &reader) {
        if (decoders::reinterpretBytes<u32_t>(reader) != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (decoders::reinterpretBytes<u32_t>(reader) != versionNumber) {
            scream("Missing or incompatible version\n");
        }

        while (!reader.eof()) {
            auto [type, size] = decoders::section(reader);
            auto pos_before = reader.getPos();

            reader.setSafeUntil(pos_before + size);

            switch (type) {
                case section::s_type:
                    decoders::vec2<module_types_t>(&types, reader, decoders::functype);
                    break;

                case section::s_import:
                    decoders::vec2<module_imports_t>(&imports, reader, decoders::importdesc);
                    break;

                case section::s_function:
                    decoders::vec2<module_functions_t>(&functions, reader, decoders::u32);
                    break;

                case section::s_table:
                    decoders::vec2<module_tables_t>(&tables, reader, decoders::table);
                    break;

                case section::s_memory:
                    decoders::vec2<module_memories_t>(&memories, reader, decoders::mem);
                    break;

                case section::s_export:
                    decoders::vec2<module_exports_t>(&exports, reader, decoders::exportdesc);
                    break;

                case section::s_code:
                    decoders::vec2<module_codes_t>(&codes, reader, decoders::code);
                    break;

                case section::s_start:
                    start = decoders::u32(reader);
                    break;

                case section::s_element:
                    decoders::vec2<module_elements_t>(&elements, reader, decoders::element);
                    break;

                case section::s_global:
                    decoders::vec2<module_globals_t>(&globals, reader, decoders::globaldesc);
                    break;

                case section::s_data:
                    decoders::vec2<module_datas_t>(&datas, reader, decoders::data);
                    break;

                default:
                    break;
            }

            reader.seekTo(pos_before + size);
        }
    }
}
