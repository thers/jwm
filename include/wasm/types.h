#pragma once

#include <stdin.h>
#include <array>
#include <vector>
#include <limits>
#include <variant>

#include <wasm/enums/opcodes.h>
#include <wasm/enums/sections.h>
#include <wasm/enums/types.h>

namespace wasm {
    template <typename... T>
    using seq_t = std::tuple<T...>;

    using size_t = uint32_t;
    using content_t = std::vector<byte_t>;

    using u64_t = uint64_t;
    using i64_t = int64_t;
    using u32_t = uint32_t;
    using i32_t = int32_t;

    using f32_t = float;
    using f64_t = double;

    using name_t = std::string;

    using memoffset = u32_t;
    using memalign = u32_t;

    template <typename T>
    using vec_t = std::vector<T>;

    struct limit_t {
        bool finite;

        u32_t min;
        u32_t max;
    };

    struct functype_t {
        vec_t<valtype> arguments_types;
        vec_t<valtype> results_types;
    };

    using index_t = u32_t;

    using typeidx_t = index_t;
    using blockidx_t = index_t;
    using labelidx_t = index_t;
    using localidx_t = index_t;
    using globalidx_t = index_t;


    using table_t = limit_t;
    using mem_t = limit_t;

    struct global_t {
        valtype val;
        bool is_mutable;
    };

    inline constexpr u32_t table_limit_max = std::numeric_limits<u32_t>::max();
    inline constexpr u32_t mem_limit_max = std::numeric_limits<uint16_t>::max();

    using importdesc_val_t = std::variant<
            index_t,
            limit_t,
            global_t
    >;
    struct importdesc_t {
        name_t module;
        name_t name;

        importtype type;
        importdesc_val_t val;
    };

    struct exportdesc_t {
        name_t export_name;

        exporttype type;
        index_t idx;
    };

    inline const u32_t magicNumber = 0x6d736100;
    inline const u32_t versionNumber = 0x1;

    typedef std::size_t pos_t;

    class result_t {
    public:
        bool empty;
        valtype type;

        result_t(valtype t): empty(t == valtype::vt_empty), type(t) {}
    };

    struct section_t {
        section type;
        size_t size;
    };

    struct memop_arg_t {
        u32_t align;
        u32_t offset;
    };

    using br_table_arg_t = std::pair<vec_t<labelidx_t>, labelidx_t>;

    using instr_arg_t = std::variant<
            nullptr_t,
            u32_t,
            u64_t,
            i32_t,
            i64_t,
            f32_t,
            f64_t,
            result_t,
            memop_arg_t,
            br_table_arg_t
    >;
    struct instr_t {
        opcode op;
        name_t opname;
        instr_arg_t arg;
    };

    using expr_t = vec_t<instr_t>;

    using constinstr_arg_t = std::variant<
            i32_t,
            i64_t,
            f32_t,
            f64_t,
            globalidx_t
    >;
    struct constinstr_t {
        constinstrtype op;
        constinstr_arg_t arg;
    };
    using constexpr_t = vec_t<constinstr_t>;

    struct local_t {
        u32_t num_locals;
        valtype type;
    };

    struct func_t {
        vec_t<local_t> locals;
        expr_t exp;
    };

    struct code_t {
        u32_t size;
        func_t func;
    };

    struct element_t {
        index_t table;
        constexpr_t offset;
        vec_t<index_t> init;
    };

    struct globaldesc_t {
        global_t type;
        constexpr_t init;
    };

    struct data_t {
        index_t data;
        constexpr_t offset;
        content_t init;
    };
}
