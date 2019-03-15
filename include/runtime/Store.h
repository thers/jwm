#pragma once

#include <wasm.h>

#include "Memory.h"
#include "Stack.h"

namespace runtime {
    class FuncInst {
        functype_t type;

    public:
        void invoke(Stack& stack);
        bool returns_void();
    };

    using store_memories_t = vec_t<Memory>;
    
    class Store {
        Stack stack;

        vec_t<FuncInst> functions;

        void invoke_function_int(index_t funcaddr, wasm::vec_t<wasm::valuettype_t>& args, wasm::valuettype_t& ret);

    public:
        template <typename T>
        T invoke_function(index_t funcaddr, wasm::vec_t<wasm::valuettype_t>& args) {
            valuettype_t ret;

            invoke_function_int(funcaddr, args, ret);

            return std::get<T>(ret);
        }
    };
}
