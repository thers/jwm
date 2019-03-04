#pragma once

#include <stdin.h>
#include <wasm.h>
#include <parser.h>
#include "sections/TypeSection.h"
#include "sections/ImportSection.h"
#include "sections/FunctionSection.h"

namespace wasm {
    class Section {
    private:
        void parseContent();

    public:
        section type;
        size_t size;
        content_t content;
        std::unique_ptr<Parser> contentParser;

        std::unique_ptr<sections::TypeSection> typeContent;
        std::unique_ptr<sections::ImportSection> importContent;
        std::unique_ptr<sections::FunctionSection> functionContent;

        void parse(Parser& parser);
    };
}
