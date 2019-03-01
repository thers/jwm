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
    using seq = std::tuple<T...>;

    using size_t = uint32_t;
    using content_t = std::vector<byte_t>;

    using u64 = uint64_t;
    using i64 = int64_t;
    using u32 = uint32_t;
    using i32 = int32_t;

    using f32 = float;
    using f64 = double;

    using name = std::string;

    using memoffset = u32;
    using memalign = u32;

    template <typename T>
    using vec = std::vector<T>;

    using limit_min = u32;
    using limit_max = u32;
    using limit_infinite = limit_min;
    using limit_finite = seq<limit_min, limit_max>;

    struct memaccess {
        u32 offset;
        u32 align;
    };
    using memop = seq<opcode, memaccess>;
    using funcdecl = seq<vec<valtype>, vec<valtype>>;

    using typeidx = u32;
    using tabletype = limit_finite;
    using memtype = limit_finite;
    struct globaltype {
        valtype t;
        mut m;
    };

    inline constexpr limit_max table_limit_max = std::numeric_limits<limit_max>::max();
    inline constexpr limit_max mem_limit_max = std::numeric_limits<uint16_t>::max();

    struct import_id {
        name module;
        name nm;
    };

    struct importdesc {
        name module;
        name nm;

        importtype type;

        typeidx x;
        tabletype tt;
        memtype mt;
        globaltype gt;
    };

    inline const u32 magicNumber = 0x0061736d;
    inline const u32 versionNumber = 0x01000000;
}
