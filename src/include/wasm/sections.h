#pragma once

#include <stdin.h>
#include <map>

namespace wasm {
    enum section : byte_t {
        s_custom = 0,
        s_type = 1,
        s_import = 2,
        s_function = 3,
        s_table = 4,
        s_memory = 5,
        s_global = 6,
        s_export = 7,
        s_start = 8,
        s_element = 9,
        s_code = 10,
        s_data = 11,
    };

    static std::map<section, const char*> section_names = {
            {s_custom, "custom"},
            {s_type, "type"},
            {s_import, "import"},
            {s_function, "function"},
            {s_table, "table"},
            {s_memory, "memory"},
            {s_global, "global"},
            {s_export, "export"},
            {s_start, "start"},
            {s_element, "element"},
            {s_code, "code"},
            {s_data, "data"},
    };
}