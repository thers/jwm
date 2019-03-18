#pragma once

#include <stack>

#include <stdin.h>
#include <wasm.h>

#include "types.h"

using namespace wasm;

namespace runtime {
    using operands_stack_t = std::stack<valuettype_t>;
    using labels_stack_t = std::stack<u32_t>;

    struct Stack {
        operands_stack_t operands;
        labels_stack_t labels;
    };
}
