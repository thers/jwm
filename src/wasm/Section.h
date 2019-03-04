#pragma once

#include <stdin.h>
#include <wasm.h>
#include <reader.h>
#include "sections/TypeSection.h"
#include "sections/ImportSection.h"
#include "sections/FunctionSection.h"
#include "sections/TableSection.h"

namespace wasm {
    class Section {
    private:
        void parseContent();

    public:
        section type;
        size_t size;
        content_t content;
        std::unique_ptr<Reader> contentParser;

        std::unique_ptr<sections::TypeSection> typeContent;
        std::unique_ptr<sections::ImportSection> importContent;
        std::unique_ptr<sections::FunctionSection> functionContent;
        std::unique_ptr<sections::TableSection> tableContent;

        void parse(Reader& reader);
    };
}
