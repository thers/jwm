#include "Module.h"

namespace wasm {
    void Module::parse(Reader &reader) {
        if (decoders::reinterpretBytes<u32_t>(reader) != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (decoders::reinterpretBytes<u32_t>(reader) != versionNumber) {
            scream("Missing or incompatible version\n");
        }

        is_valid = true;

        print("Module is valid, reading sections, pointer at %d\n", reader.get_pos());

        while (!reader.eof()) {
            auto [type, size] = decoders::section(reader);
            Reader sectionReader(reader.content(size));

            switch (type) {
                case wasm::section::s_type:
                    typeSection = std::make_unique<sections::TypeSection>(sectionReader);
                    break;

                case wasm::section::s_import:
                    importSection = std::make_unique<sections::ImportSection>(sectionReader);
                    break;

                case wasm::section::s_function:
                    functionSection = std::make_unique<sections::FunctionSection>(sectionReader);
                    break;

                case wasm::section::s_table:
                    tableSection = std::make_unique<sections::TableSection>(sectionReader);
                    break;

                case wasm::section::s_memory:
                    memorySection = std::make_unique<sections::MemorySection>(sectionReader);
                    break;
            }
        }
    }
}
