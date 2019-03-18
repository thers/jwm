#include <runtime/Store.h>

namespace runtime {
    global_t* GlobalInst::get_desc() {
        return &desc;
    }

    constinstr_arg_t GlobalInst::get() {
        return value;
    }

    void GlobalInst::set(constinstr_arg_t v) {
        if (!desc.is_mutable) {
            scream("Attempt to modify immutable global");
        }

        value = v;
    }

    Store::Store(Module& module) {

    }

    constinstr_arg_t Store::get_global(index_t index) {
        return globals[index].get_desc()->val;
    }
}
