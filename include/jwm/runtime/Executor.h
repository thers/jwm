#pragma once

#include <jwm/wasm.h>

#include "Store.h"
#include "Module.h"

using namespace jwm;

namespace jwm::runtime::executor {
    inline val_t constexprEval(runtime::Store &store, ModuleInst &moduleInst, constexpr_decl_t &expr) {
        operands_stack_t stack;

        for (auto[op, arg]: expr) {
            switch (op) {
                case constinstrtype::cit_i32:
                    stack.push(std::get<i32_t>(arg));
                    break;

                case constinstrtype::cit_i64:
                    stack.push(std::get<i64_t>(arg));
                    break;

                case constinstrtype::cit_f32:
                    stack.push(std::get<f32_t>(arg));
                    break;

                case constinstrtype::cit_f64:
                    stack.push(std::get<f64_t>(arg));
                    break;

                case constinstrtype::cit_global_get: {
                    auto global = store.get_global(moduleInst, std::get<index_decl_t>(arg));

                    std::visit(overloaded{
                            [&](auto v) { scream("Error during global initialization"); },
                            [&](i32_t v) { stack.push(v); },
                            [&](i64_t v) { stack.push(v); },
                            [&](f32_t v) { stack.push(v); },
                            [&](f64_t v) { stack.push(v); },
                    }, global);

                    break;
                }

                case constinstrtype::cit_end:
                    scream("Unexpected end op");
                    break;

                default:
                    scream("Unexpected constexpr op");
                    break;
            }
        }

        if (stack.empty()) {
            scream("Invalid constexpr initialization sequence, missing end op");
        }

        return stack.top();
    }
}
