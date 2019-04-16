#pragma once

#include <array>
#include <vector>
#include <limits>
#include <variant>
#include <utility>

#include <jwm/stdinc.h>

#include "enums/opcodes.h"
#include "enums/sections.h"
#include "enums/types.h"

namespace jwm::wasm {
    using addr_t = uint64_t;

    template<typename... T>
    using seq_t = std::tuple<T...>;

    template<typename T>
    using span_t = std::pair<T, size_t>;

    using content_t = std::vector<byte_t>;

    using u64_t = uint64_t;
    using i64_t = int64_t;
    using u32_t = uint32_t;
    using i32_t = int32_t;

    using f32_t = float;
    using f64_t = double;

    using name_t = std::string;

    inline const u32_t magicNumber = 0x6d736100;
    inline const u32_t versionNumber = 0x1;

    using memoffset = u32_t;
    using memalign = u32_t;

    template<typename T>
    using vec_t = std::vector<T>;

    struct limit_decl_t {
        bool finite;

        u32_t min;
        u32_t max;
    };

    struct func_type_decl_t {
        vec_t<valtype> arguments_types;
        vec_t<valtype> results_types;
    };

    using valuettype_t = std::variant<i32_t, i64_t, f32_t, f64_t>;

    using index_decl_t = u32_t;

    using typeidx_decl_t = index_decl_t;
    using blockidx_decl_t = index_decl_t;
    using labelidx_decl_t = index_decl_t;
    using localidx_decl_t = index_decl_t;
    using globalidx_decl_t = index_decl_t;


    using table_decl_t = limit_decl_t;
    using mem_decl_t = limit_decl_t;

    struct global_type_decl_t {
        valtype val;
        bool is_mutable;
    };

    inline constexpr u32_t table_limit_max = std::numeric_limits<u32_t>::max();
    inline constexpr u32_t mem_limit_max = std::numeric_limits<uint16_t>::max();

    using import_decl_val_t = std::variant<index_decl_t, limit_decl_t, global_type_decl_t>;
    struct import_decl_t {
        name_t module;
        name_t name;

        importtype type;
        import_decl_val_t val;
    };

    struct export_decl_t {
        name_t export_name;

        exporttype type;
        index_decl_t idx;
    };

    struct export_inst_t {
        addr_t addr;
        name_t name;
        exporttype type;
    };

    typedef std::size_t pos_t;

    class result_decl_t {
    public:
        bool empty;
        valtype type;

        result_decl_t(valtype t) : empty(t == valtype::vt_empty), type(t) {}
    };

    struct section_decl_t {
        section type;
        u32_t size;
    };

    struct memory_immediate_arg_decl_t {
        u32_t align;
        u32_t offset;
    };

    using br_table_arg_t = std::pair<vec_t<labelidx_decl_t>, labelidx_decl_t>;

    using val_t = std::variant<i32_t, i64_t, f32_t, f64_t>;

    using instr_arg_decl_t = std::optional<std::variant<i32_t, i64_t, f32_t, f64_t, u32_t, u64_t,
            result_decl_t, memory_immediate_arg_decl_t, br_table_arg_t>>;
    struct instr_decl_t {
        opcode op;
        name_t opname;
        instr_arg_decl_t arg;
    };

    using expr_decl_t = vec_t<instr_decl_t>;

    using constinstr_arg_decl_t = std::variant<i32_t, i64_t, f32_t, f64_t, globalidx_decl_t>;
    struct constinstr_decl_t {
        constinstrtype op;
        constinstr_arg_decl_t arg;
    };
    using constexpr_decl_t = vec_t<constinstr_decl_t>;

    struct locals_decl_t {
        u32_t num_locals;
        valtype type;
    };

    struct func_decl_t {
        vec_t<locals_decl_t> locals;
        expr_decl_t exp;
    };

    struct code_decl_t {
        const u32_t size;
        const func_decl_t func;
    };

    struct element_decl_t {
        index_decl_t table;
        constexpr_decl_t offset;
        vec_t<index_decl_t> init;
    };

    struct global_decl_t {
        valtype type;
        bool mut;
        constexpr_decl_t init;

        global_decl_t(global_type_decl_t &t, constexpr_decl_t &i) :
                type(t.val),
                mut(t.is_mutable),
                init(i) {}
    };

    struct data_t {
        index_decl_t data;
        constexpr_decl_t offset;
        content_t init;
    };

    using args_t = vec_t<val_t>;
}
