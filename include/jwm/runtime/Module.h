#pragma once

#include <map>

#include <jwm/stdinc.h>
#include <jwm/wasm.h>

using namespace jwm::wasm;

namespace jwm::runtime {
    class Module {
    public:
        using module_types_t = vec_t<func_args_decl_t>;
        using module_imports_t = vec_t<import_decl_t>;
        using module_functions_t = vec_t<u32_t>;
        using module_tables_t = vec_t<table_decl_t>;
        using module_memories_t = vec_t<mem_decl_t>;
        using module_exports_t = vec_t<export_decl_t>;
        using module_codes_t = vec_t<code_decl_t>;
        using module_start_t = u32_t;
        using module_elements_t = vec_t<element_decl_t>;
        using module_globals_t = vec_t<global_decl_t>;
        using module_datas_t = vec_t<data_t>;
    private:
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
        Module(content_t &content);

        global_decl_t *get_global(index_decl_t index);

        template<typename T>
        void for_each_global(T cb) {
            for (auto global: globals) {
                cb(global);
            }
        }
    };

    class ModuleInst {
    public:
        using moduleinst_types_t = Module::module_types_t;
        using moduleinst_funcaddr_t = vec_t<addr_t>;
        using moduleinst_tableaddr_t = vec_t<addr_t>;
        using moduleinst_memaddr_t = vec_t<addr_t>;
        using moduleinst_globaladdr_t = vec_t<addr_t>;
        using moduleinst_exportinst_t = vec_t<export_inst_t>;
        using moduleinst_map_t = std::map<name_t, index_decl_t>;
    private:
        Module::module_types_t types;
        moduleinst_funcaddr_t funcaddr;
        moduleinst_tableaddr_t tableaddr;
        moduleinst_memaddr_t memaddr;
        moduleinst_globaladdr_t globaladdr;
        moduleinst_exportinst_t exportinst;
        moduleinst_map_t globals_map;
    public:
        index_decl_t add_type(func_args_decl_t &type);

        index_decl_t add_func(addr_t addr);

        index_decl_t add_table(addr_t addr);

        index_decl_t add_memory(addr_t addr);

        index_decl_t add_global(addr_t addr, name_t &name);

        index_decl_t add_export(export_inst_t &einst);

        func_args_decl_t get_type(index_decl_t index);

        addr_t get_func(index_decl_t index);

        addr_t get_table(index_decl_t index);

        addr_t get_memory(index_decl_t index);

        addr_t get_global(index_decl_t index);

        addr_t get_global(name_t &name);

        export_inst_t get_export(index_decl_t index);
    };
}
