#pragma once

#include <jwm/wasm.h>

#include "Store.h"
#include "Module.h"

using namespace jwm;

namespace jwm::runtime::executor {
    val_t constexprEval(runtime::Store &store, ModuleInst &moduleInst, constexpr_decl_t &expr);
    val_t exprEval(runtime::Store &$store, ModuleInst &moduleInst, expr_decl_t &expr);
}
