#include "Section.h"
#include "sections/TypeSection.h"

namespace wasm {
    static const bool DEBUG = false;

    void Section::parse(Parser &parser) {
        if (DEBUG) print("\nSection\n");
        if (DEBUG) print("\tpos 0x%08X\n", parser.get_pos());

        type = static_cast<section>(parser.read());
        if (DEBUG) print("\ttype %02X[%s]\n", type, section_names[type]);

        size = parser.readUnsigned<u32>();
        if (DEBUG) print("\tsize %d\n", size);

        if (DEBUG) print("\tcontent pos %08X\n", parser.get_pos());
        content = parser.read(size);
        if (DEBUG) print("\tafter content pos %08X\n", parser.get_pos());

        if (DEBUG) print("-----------------------------\n");
        if (DEBUG) printv(content);
        if (DEBUG) print("\n-----------------------------\n");

        contentParser = std::make_unique<Parser>(content);
        parseContent();
    }

    void Section::parseContent() {
        switch (type) {
            case wasm::section::s_type:
                typeContent = std::make_unique<sections::TypeSection>(size, *contentParser.get());
                break;
        }
    }
}
