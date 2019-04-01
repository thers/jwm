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

//    val_t Store::get_global(ModuleInst &moduleInst, name_t name) {
//        return globals[moduleInst.get_global(name)].get_value();
//    }

    ModuleInst Store::allocate_module(ModuleInst &globalInst, Module &module) {
        ModuleInst inst;

        // Evaluating globals from globals module inst
        module.for_each_global([&](global_decl_t globalDesc) {
            auto value = executor::constexprEval((*this), globalInst, globalDesc.init);

            inst.add_global(globals.size());
            globals.push_back({globalDesc, value});
        });

        module.for_each_function([&](func_args_decl_t &args, code_decl_t &code) {
            inst.add_func(functions.size());
            functions.push_back({args, code});
        });

        module.for_each_table([&](table_decl_t &table) {
            inst.add_table(tables.size());
            tables.push_back({table});
        });

        module.for_each_memory([&](mem_decl_t &memory) {
            Memory mem(memory, trap);

            inst.add_memory(memories.size());
            memories.push_back(mem);
        });

        module.for_each_element([&](element_decl_t &element) {
            auto value = executor::constexprEval((*this), inst, element.offset);
            auto table = tables[element.table];
        });

        // Populating exports
        module.for_each_export([&](export_decl_t &exprt) {
            addr_t address;

           switch (exprt.type) {
               case exporttype::et_funcidx:
                   address = inst.get_func(exprt.idx);
                   break;

               case exporttype::et_globalidx:
                   address = inst.get_global(exprt.idx);
                   break;

               case exporttype::et_memidx:
                   address = inst.get_memory(exprt.idx);
                   break;

               default:
               case exporttype::et_tableidx:
                   address = inst.get_table(exprt.idx);
                   break;
           }

           inst.add_export(address, exprt.export_name, exprt.type);
        });

        return inst;
    }
}
