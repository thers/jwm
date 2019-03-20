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

    val_t Store::get_global(ModuleInst &moduleInst, index_t index) {
        return globals[moduleInst.get_global(index)].get_value();
    }

    val_t Store::get_global(ModuleInst &moduleInst, name_t name) {
        return globals[moduleInst.get_global(name)].get_value();
    }

    void Store::allocate_module(ModuleInst &globalInst, Module &module) {
        module.for_each_global([&](globaldesc_t globalDesc) {
            auto value = executor::constexprEval((*this), globalInst, globalDesc.init);

            globals.push_back({globalDesc.type, value});
        });
    }
}
