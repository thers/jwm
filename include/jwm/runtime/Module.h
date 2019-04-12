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

        index_decl_t get_start();

        template<typename T>
        void for_each_global(T cb) {
            for (auto global: globals) {
                cb(global);
            }
        }

        template<typename T>
        void for_each_function(T cb) {
            for (auto function: functions) {
                cb(types[function], codes[function]);
            }
        }

        template<typename T>
        void for_each_table(T cb) {
            for (auto table: tables) {
                cb(table);
            }
        }

        template<typename T>
        void for_each_memory(T cb) {
            for (auto memory: memories) {
                cb(memory);
            }
        }

        template<typename T>
        void for_each_element(T cb) {
            for (auto element: elements) {
                cb(element);
            }
        }

        template<typename T>
        void for_each_export(T cb) {
            for (auto exprt: exports) {
                cb(exprt);
            }
        }

        template<typename T>
        void for_each_data(T cb) {
            for (auto data: datas) {
                cb(data);
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
    private:
        Module::module_types_t types;
        moduleinst_funcaddr_t funcaddr;
        moduleinst_tableaddr_t tableaddr;
        moduleinst_memaddr_t memaddr;
        moduleinst_globaladdr_t globaladdr;
        moduleinst_exportinst_t exportinst;
    public:
        void add_type(func_args_decl_t &type);

        void add_func(addr_t addr);

        void add_table(addr_t addr);

        void add_memory(addr_t addr);

        void add_global(addr_t addr);

        void add_export(addr_t addr, name_t &name, exporttype type);

        func_args_decl_t get_type(index_decl_t index);

        addr_t get_func(index_decl_t index);

        addr_t get_table(index_decl_t index);

        addr_t get_memory(index_decl_t index);

        addr_t get_global(index_decl_t index);
    };
}
