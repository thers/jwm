#pragma once

#include <stdin.h>
#include <array>
#include <vector>
#include <limits>

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

    using f32 = float;
    using f64 = double;

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

    struct memaccess_t {
        u32_t offset;
        u32_t align;
    };
    using memop_t = seq_t<opcode, memaccess_t>;

    struct func_t {
        vec_t<valtype> arguments_types;
        vec_t<valtype> results_types;
    };

    using typeidx_t = u32_t;
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

    struct section_t {
        section type;
        size_t size;
    };
}
