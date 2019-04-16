#include <jwm/jwm.h>

namespace jwm {
    ModuleInstance::ModuleInstance(const content_t &bytes, const ModuleImports &imports)
    : module(bytes),
      store(),
      inst()
    {
        runtime::ModuleInst tempGlobalInst;
        store.allocate_module(tempGlobalInst, module, inst);
    }

    val_t ModuleInstance::start(vec_t<val_t> &args) {
        return store.start(inst, module, args);
    }
}
