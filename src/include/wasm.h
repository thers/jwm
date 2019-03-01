#pragma once

#include <stdin.h>
#include <array>
#include <vector>

#include <wasm/opcodes.h>
#include <wasm/sections.h>
#include <wasm/types.h>

namespace wasm {
    typedef uint64_t u64;
    typedef int64_t i64;
    typedef uint32_t u32;
    typedef int32_t i32;

    typedef float f32;
    typedef double f64;

    typedef byte_t id_t;
    typedef uint32_t size_t;
    typedef std::vector<byte_t> content_t;

    using memoffset = u32;
    using memalign = u32;

    template <typename T>
    using vec = std::vector<T>;

    using memop = std::tuple<opcode, memoffset, memalign>;
    using funcdecl = std::tuple<vec<valtype>, vec<valtype>>;

    static const u32 magicNumber = 0x0061736d;
    static const u32 versionNumber = 0x01000000;
}
