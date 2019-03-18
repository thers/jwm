#pragma once

#include <wasm.h>

#include "Module.h"
#include "Memory.h"
#include "Stack.h"

using namespace wasm;

namespace runtime {
    class FuncInst {
        functype_t type;
    public:
    };

    class GlobalInst {
        global_t desc;
        constinstr_arg_t value;
    public:
        global_t* get_desc();
        constinstr_arg_t get();
        void set(constinstr_arg_t v);
    };

    class Store {
        Stack stack;
        vec_t<FuncInst> functions;
        vec_t<GlobalInst> globals;
    public:
        Store(Module& module);
        constinstr_arg_t get_global(index_t index);
    };
}
