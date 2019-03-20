#pragma once

#include <jwm/wasm.h>

#include "Module.h"
#include "Memory.h"
#include "Stack.h"

using namespace jwm::wasm;

namespace jwm::runtime {
    class FuncInst {
        functype_t type;
    public:
    };

    class GlobalInst {
        bool mut;
        val_t value;
    public:
        GlobalInst(global_t &desc, val_t v) :
                mut(desc.is_mutable),
                value(v) {}

        val_t get_value();

        void set(val_t v);
    };

    class Store {
        Stack stack;
        vec_t<FuncInst> functions;
        vec_t<GlobalInst> globals;

    public:
        val_t get_global(ModuleInst &moduleInst, index_t index);

        val_t get_global(ModuleInst &moduleInst, name_t name);

        void allocate_module(ModuleInst &globalInst, Module &module);
    };
}
