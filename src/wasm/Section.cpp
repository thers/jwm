#include "Section.h"

namespace wasm {
    void Section::parse(Reader& reader) {
        type = decoders::byteEnumItem<section>(reader);
        size = decoders::u32(reader);
        content = reader.content(size);
        contentParser = std::make_unique<Reader>(content);

        parseContent();
    }

    void Section::parseContent() {
        switch (type) {
            case wasm::section::s_type:
                typeContent = std::make_unique<sections::TypeSection>(*contentParser);
                break;

            case wasm::section::s_import:
                importContent = std::make_unique<sections::ImportSection>(*contentParser);
                break;

            case wasm::section::s_function:
                functionContent = std::make_unique<sections::FunctionSection>(*contentParser);
                break;
        }
    }
}
