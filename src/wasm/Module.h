#pragma once

#include <stdin.h>
#include <wasm.h>
#include <decoders.h>

using namespace std;

namespace wasm {
    using module_types_t = vec_t<functype_t>;
    using module_imports_t = vec_t<importdesc_t>;
    using module_functions_t = vec_t<u32_t>;
    using module_tables_t = vec_t<table_t>;
    using module_memories_t = vec_t<mem_t>;
    using module_exports_t = vec_t<exportdesc_t>;
    using module_codes_t = vec_t<code_t>;
    using module_start_t = u32_t;
    using module_element_t = vec_t<element_t>;

    class Module {
        module_types_t types;
        module_imports_t imports;
        module_functions_t functions;
        module_tables_t tables;
        module_memories_t memories;
        module_exports_t exports;
        module_codes_t codes;
        module_start_t start;
        module_element_t elements;

    public:
        Module(Reader& reader);
    };
}
