#pragma once

#include <jwm/wasm.h>

#include "Store.h"
#include "Module.h"

using namespace jwm;

namespace jwm::runtime::executor {
    val_t constexprEval(runtime::Store &store, ModuleInst &moduleInst, const constexpr_decl_t &expr);

    val_t functionEval(
            runtime::Store &store,
            ModuleInst &moduleInst,
            const func_decl_t &func,
            const args_t &args
    );
}
