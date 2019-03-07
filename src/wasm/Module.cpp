#include "Module.h"

namespace wasm {
    void Module::parse(Reader &reader) {
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
                    typeSection = std::make_unique<sections::TypeSection>(reader);
                    break;

                case wasm::section::s_import:
                    importSection = std::make_unique<sections::ImportSection>(reader);
                    break;

                case wasm::section::s_function:
                    functionSection = std::make_unique<sections::FunctionSection>(reader);
                    break;

                case wasm::section::s_table:
                    tableSection = std::make_unique<sections::TableSection>(reader);
                    break;

                case wasm::section::s_memory:
                    memorySection = std::make_unique<sections::MemorySection>(reader);
                    break;

                case wasm::section::s_export:
                    exportSection = std::make_unique<sections::ExportSection>(reader);
                    break;

                case wasm::section::s_code:
                    codeSection = std::make_unique<sections::CodeSection>(reader);
                    break;

                case wasm::section::s_start:
                    startSection = std::make_unique<sections::StartSection>(reader);
                    break;
            }

            print("done.\n");

            reader.seek_to(pos_before + size);
        }
    }
}
