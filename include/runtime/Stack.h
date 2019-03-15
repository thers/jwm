#pragma once

#include <stack>

#include <stdin.h>
#include <wasm.h>

#include "types.h"

namespace runtime {
    struct Stack {
        std::stack<wasm::valuettype_t> operands;
        std::stack<wasm::u32_t> labels;
    };
}
