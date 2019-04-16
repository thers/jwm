#pragma once

#include <jwm/wasm.h>

#include "Module.h"
#include "Memory.h"
#include "Stack.h"

using namespace jwm::wasm;

namespace jwm::runtime {
    class FuncInst {
        const func_type_decl_t type;
        const code_decl_t code;
    public:
        FuncInst(func_type_decl_t &t, code_decl_t &c) :
                type(t),
                code(c) {}

        const func_decl_t get_func();
        const func_type_decl_t get_type();
    };

    class GlobalInst {
        bool mut;
        val_t value;
    public:
        GlobalInst(global_decl_t &decl, val_t v) :
                mut(decl.mut),
                value(v) {}

        val_t get_value();

        void set(val_t v);
    };

    class TableInst {
        type elemtype;
        limit_decl_t limits;
        vec_t<addr_t> elem;
    public:
        TableInst(limit_decl_t &l) :
                elemtype(type::t_funcref),
                limits(l) {}

        void set_elem(u32_t at, addr_t address);
    };

    class ExportInst {

    };

    class Store {
        Stack stack;
        vec_t<FuncInst> functions;
        vec_t<GlobalInst> globals;
        vec_t<TableInst> tables;
        vec_t<std::unique_ptr<Memory>> memories;
        vec_t<ExportInst> exports;
    public:
        static void trap();

        val_t get_global(ModuleInst &moduleInst, index_decl_t index);

        val_t get_global(ModuleInst &moduleInst, name_t name);

        FuncInst get_func(ModuleInst &moduleInst, index_decl_t index);

        void allocate_module(ModuleInst &globalInst, Module &module, ModuleInst &inst);

        val_t start(ModuleInst &moduleInst, Module &module, args_t &args);
    };
}
