#pragma once

#include "wasm.h"
#include "wasm.h"
#include "runtime.h"

#include "ModuleImports.h"

namespace jwm {
    class ModuleInstance {
        runtime::Store store;
        runtime::Module module;
        runtime::ModuleInst inst;
    public:
        ModuleInstance(const content_t &bytes, const ModuleImports &imports);

        val_t start(vec_t<val_t> &args);
    };
}
