#pragma once

#include <stdin.h>
#include <wasm.h>

#include "sections/TypeSection.h"
#include "sections/ImportSection.h"
#include "sections/FunctionSection.h"
#include "sections/TableSection.h"
#include "sections/MemorySection.h"
#include "sections/ExportSection.h"
#include "sections/CodeSection.h"

namespace wasm {
    class Module {
        std::unique_ptr<sections::TypeSection> typeSection;
        std::unique_ptr<sections::ImportSection> importSection;
        std::unique_ptr<sections::FunctionSection> functionSection;
        std::unique_ptr<sections::TableSection> tableSection;
        std::unique_ptr<sections::MemorySection> memorySection;
        std::unique_ptr<sections::ExportSection> exportSection;
        std::unique_ptr<sections::CodeSection> codeSection;

    public:
        void parse(Reader& reader);
    };
}
