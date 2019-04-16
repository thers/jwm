#include <jwm/jwm.h>

#include "reader.h"
#include "decoders.h"

namespace jwm {
    runtime::Module decode_module(content_t &content) {
        return {content};
    }

    void parse_module(std::string &content) {
        scream("Text format is not supported, use dedicated utilities to compile binary");
    }
}

namespace jwm::runtime {
    Module::Module(content_t &content) {
        Reader reader(content);

        if (decoders::reinterpretBytes<u32_t>(reader) != magicNumber) {
            scream("Magic number is invalid\n");
        }

        if (decoders::reinterpretBytes<u32_t>(reader) != versionNumber) {
            scream("Missing or incompatible version\n");
        }

        while (!reader.eof()) {
            auto[type, size] = decoders::section(reader);
            auto pos_before = reader.getPos();

            reader.setSafeUntil(pos_before + size);

            switch (type) {
                case section::s_type:
                    decoders::vec2<module_types_t>(&types, reader, decoders::functype);
                    break;

                case section::s_import:
                    decoders::vec2<module_imports_t>(&imports, reader, decoders::importdesc);
                    break;

                case section::s_function:
                    decoders::vec2<module_functions_t>(&functions, reader, decoders::u32);
                    break;

                case section::s_table:
                    decoders::vec2<module_tables_t>(&tables, reader, decoders::table);
                    break;

                case section::s_memory:
                    decoders::vec2<module_memories_t>(&memories, reader, decoders::mem);
                    break;

                case section::s_export:
                    decoders::vec2<module_exports_t>(&exports, reader, decoders::exportdesc);
                    break;

                case section::s_code:
                    decoders::vec2<module_codes_t>(&codes, reader, decoders::code);
                    break;

                case section::s_start:
                    start = decoders::u32(reader);
                    break;

                case section::s_element:
                    decoders::vec2<module_elements_t>(&elements, reader, decoders::element);
                    break;

                case section::s_global:
                    decoders::vec2<module_globals_t>(&globals, reader, decoders::globaldesc);
                    break;

                case section::s_data:
                    decoders::vec2<module_datas_t>(&datas, reader, decoders::data);
                    break;

                default:
                    break;
            }

            reader.seekTo(pos_before + size);
        }
    }

    global_decl_t *Module::get_global(index_decl_t index) {
        return &globals[index];
    }

    index_decl_t Module::get_start() {
        return start;
    }

    void ModuleInst::add_type(func_type_decl_t &type) {
        types.push_back(type);
    }

    void ModuleInst::add_func(addr_t addr) {
        funcaddr.push_back(addr);
    }

    void ModuleInst::add_table(addr_t addr) {
        tableaddr.push_back(addr);
    }

    void ModuleInst::add_memory(addr_t addr) {
        memaddr.push_back(addr);
    }

    void ModuleInst::add_global(addr_t addr) {
        globaladdr.push_back(addr);
    }

    void ModuleInst::add_export(addr_t addr, name_t &name, exporttype type) {
        exportinst.push_back({addr, name, type});
    }

    func_type_decl_t ModuleInst::get_type(index_decl_t index) {
        return types[index];
    }

    addr_t ModuleInst::get_func(index_decl_t index) {
        return funcaddr[index];
    }

    addr_t ModuleInst::get_table(index_decl_t index) {
        return tableaddr[index];
    }

    addr_t ModuleInst::get_memory(index_decl_t index) {
        return memaddr[index];
    }

    addr_t ModuleInst::get_global(index_decl_t index) {
        return globaladdr[index];
    }
}
