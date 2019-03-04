#include "Module.h"

namespace wasm {
    void Module::parse(Parser &parser) {
        if (parser.read<u32>() != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (parser.read<u32>() != versionNumber) {
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
