#pragma once

#include <map>

#include <stdin.h>
#include <wasm.h>

using namespace wasm;

namespace runtime {
    using module_types_t = vec_t<functype_t>;
    using module_imports_t = vec_t<importdesc_t>;
    using module_functions_t = vec_t<u32_t>;
    using module_tables_t = vec_t<table_t>;
    using module_memories_t = vec_t<mem_t>;
    using module_exports_t = vec_t<exportdesc_t>;
    using module_codes_t = vec_t<code_t>;
    using module_start_t = u32_t;
    using module_elements_t = vec_t<element_t>;
    using module_globals_t = vec_t<globaldesc_t>;
    using module_datas_t = vec_t<data_t>;

    class Module {
        module_types_t types;
        module_imports_t imports;
        module_functions_t functions;
        module_tables_t tables;
        module_memories_t memories;
        module_globals_t globals;
        module_exports_t exports;
        module_elements_t elements;
        module_codes_t codes;
        module_datas_t datas;
        module_start_t start = 0;

    public:
        Module(Reader& reader);

        u32_t get_num_functions();

        bool function_exist();

        globaldesc_t* get_global(index_t index);
    };

    using moduleinst_types_t = module_types_t;
    using moduleinst_funcaddr_t = vec_t<addr_t>;
    using moduleinst_tableaddr_t = vec_t<addr_t>;
    using moduleinst_memaddr_t = vec_t<addr_t>;
    using moduleinst_globaladdr_t = vec_t<addr_t>;
    using moduleinst_exportinst_t = vec_t<exportinst_t>;

    using moduleinst_map_t = std::map<name_t, index_t>;

    class ModuleInst {
        module_types_t types;
        moduleinst_funcaddr_t funcaddr;
        moduleinst_tableaddr_t tableaddr;
        moduleinst_memaddr_t memaddr;
        moduleinst_globaladdr_t globaladdr;
        moduleinst_exportinst_t exportinst;

        moduleinst_map_t globals_map;
    public:
        index_t add_type(functype_t type);
        index_t add_func(addr_t addr);
        index_t add_table(addr_t addr);
        index_t add_memory(addr_t addr);
        index_t add_global(addr_t addr, name_t name);
        index_t add_export(exportinst_t einst);

        functype_t get_type(index_t index);
        addr_t get_func(index_t index);
        addr_t get_table(index_t index);
        addr_t get_memory(index_t index);
        addr_t get_global(index_t index);
        addr_t get_global(name_t name);
        exportinst_t get_export(index_t index);
    };
}
