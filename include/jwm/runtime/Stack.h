#pragma once

#include <stack>

#include <jwm/stdinc.h>
#include <jwm/wasm.h>

#include "types.h"

using namespace jwm::wasm;

namespace jwm::runtime {
    using operands_stack_t = std::stack<valuettype_t>;
    using labels_stack_t = std::stack<u32_t>;

    struct Stack {
        operands_stack_t operands;
        labels_stack_t labels;
    };
}
