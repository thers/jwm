#pragma once

#include <stdin.h>
#include <wasm.h>

#include "sections/TypeSection.h"
#include "sections/ImportSection.h"
#include "sections/FunctionSection.h"
#include "sections/TableSection.h"
#include "sections/MemorySection.h"

namespace wasm {
    class Module {
        std::unique_ptr<sections::TypeSection> typeSection;
        std::unique_ptr<sections::ImportSection> importSection;
        std::unique_ptr<sections::FunctionSection> functionSection;
        std::unique_ptr<sections::TableSection> tableSection;
        std::unique_ptr<sections::MemorySection> memorySection;

    protected:
        bool is_valid;

    public:
        Module(): is_valid(false) {}

        void parse(Reader& reader);
    };
}
