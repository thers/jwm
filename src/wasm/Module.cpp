#include "Module.h"

namespace wasm {
    void Module::parse(Reader &parser) {
        if (decoders::reinterpretBytes<u32_t>(parser) != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (decoders::reinterpretBytes<u32_t>(parser) != versionNumber) {
            scream("Missing or incompatible version\n");
        }

        is_valid = true;

        print("Module is valid, reading sections, pointer at %d\n", parser.get_pos());

        while (!parser.eof()) {
            auto section = std::make_unique<Section>();

            section->parse(parser);

            sections.push_back(std::move(section));
        }
    }
}
