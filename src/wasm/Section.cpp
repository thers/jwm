#include "Section.h"
#include "sections/TypeSection.h"

namespace wasm {
    void Section::parse(Parser &parser) {
        type = static_cast<section>(parser.read());
        size = parser.read_u32();
        content = parser.read(size);
        contentParser = std::make_unique<Parser>(content);

        parseContent();
    }

    void Section::parseContent() {
        auto cp = *contentParser.get();

        switch (type) {
            case wasm::section::s_type:
                typeContent = std::make_unique<sections::TypeSection>(size, *contentParser.get());
                break;

            case wasm::section::s_import:
                importContent = std::make_unique<sections::ImportSection>(size, *contentParser.get());
                break;
        }
    }
}
