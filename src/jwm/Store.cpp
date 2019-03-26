#include <jwm/runtime.h>

namespace jwm::runtime {
    val_t GlobalInst::get_value() {
        return value;
    }

    void GlobalInst::set(val_t v) {
        if (!mut) {
            scream("Attempt to modify immutable global");
        }

        value = v;
    }

    void Store::trap() {
        scream("Unexpected trap occured");
    }

    val_t Store::get_global(ModuleInst &moduleInst, index_decl_t index) {
        return globals[moduleInst.get_global(index)].get_value();
    }

    val_t Store::get_global(ModuleInst &moduleInst, name_t name) {
        return globals[moduleInst.get_global(name)].get_value();
    }

    void Store::allocate_module(ModuleInst &globalInst, Module &module) {
        module.for_each_global([&](global_decl_t globalDesc) {
            auto value = executor::constexprEval((*this), globalInst, globalDesc.init);

            globals.push_back({globalDesc, value});
        });

        ModuleInst inst;

        module.for_each_function([&](func_args_decl_t &args, code_decl_t &code) {
           functions.push_back({args, code});
        });

        module.for_each_table([&](table_decl_t &table) {
            tables.push_back({table});
        });

        module.for_each_memory([&](mem_decl_t &memory) {
            Memory mem(memory, trap);

            memories.push_back(mem);
        });
    }
}
