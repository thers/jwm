#pragma once

#include <stdin.h>
#include <parser.h>
#include <wasm.h>

#include "Section.h"

namespace wasm {
    class Module {
    protected:
        bool is_valid;

    public:
        std::vector<std::unique_ptr<Section>> sections;

        Module(): is_valid(false) {}

        void parse(Parser& parser);
    };
}
