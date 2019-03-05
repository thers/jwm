#pragma once

#include <stdin.h>
#include <array>
#include <vector>
#include <limits>
#include <variant>

#include <wasm/opcodes.h>
#include <wasm/sections.h>
#include <wasm/types.h>

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
        valtype value_type;
        bool is_mutable;
    };

    inline constexpr u32_t table_limit_max = std::numeric_limits<u32_t>::max();
    inline constexpr u32_t mem_limit_max = std::numeric_limits<uint16_t>::max();

    struct importdesc_t {
        name_t module;
        name_t nm;

        importtype type;

        typeidx_t x;
        table_t tt;
        mem_t mt;
        global_t gt;
    };

    inline const u32_t magicNumber = 0x0061736d;
    inline const u32_t versionNumber = 0x01000000;

    typedef std::size_t pos_t;

    class Reader {
    private:
        const content_t data;
        pos_t size;
        pos_t pos;

    public:
        Reader(const content_t c): data(c), size(c.size()), pos(0) {}

        bool eof() {
            return pos >= size;
        }

        pos_t get_pos() {
            return pos;
        }

        content_t get_content() {
            return {data.begin() + pos,data.end()};
        }

        byte_t next() {
            return data[pos++];
        }

        void seek_to(pos_t p) {
            pos = p;
        }

        content_t content(int length) {
            auto from = data.begin() + pos;
            auto to = from + length;

            pos += length;

            return {from, to};
        }
    };

    struct result_t {
        bool empty;
        valtype type;
    };

    inline result_t default_result_t;

    struct section_t {
        section type;
        size_t size;
    };

    struct memop_arg_t {
        u32_t offset;
        u32_t align;
    };

    using br_table_arg_t = std::pair<vec_t<labelidx_t>, labelidx_t>;

    using instr_arg_t = std::variant<
            nullptr_t,
            u32_t,
            u64_t,
            memop_arg_t,
            br_table_arg_t
    >;
    struct instr_t {
        opcode op;
        instr_arg_t arg;
    };

    using expr_t = vec_t<instr_t>;
    struct block_t {
        result_t result;
        expr_t exp;
    };

    struct expression_t {
        expr_t main;
        vec_t<block_t> blocks;
    };

    using constexpression_t = std::pair<constinstrtype, std::variant<
            i32_t,
            i64_t,
            f32_t,
            f64_t,
            globalidx_t
    >>;

    struct local_t {
        u32_t n;
        valtype t;
    };

    struct func_t {
        vec_t<local_t> locals;
        expression_t exp;
    };

    struct code_t {
        u32_t size;
        func_t func;
    };
}
