#pragma once

#include <cstdlib>

#include <stdin.h>
#include <wasm.h>

#include "types.h"

using namespace wasm;

namespace runtime {
    class Memory {
        static const u32_t page_size = 65536;

        byte_t *m_ptr;
        u32_t size;
        u32_t max;

        u32_t max_offset;

        trap_t trap;

        void check_boundaries(u32_t at);

    public:
        explicit Memory(u32_t s, u32_t m, trap_t t):
            size(s),
            max(m),
            max_offset(s * page_size),
            trap(t),
            m_ptr(reinterpret_cast<byte_t*>(std::malloc(s * page_size)))
        {}

        ~Memory() { std::free(m_ptr); }

        void grow(u32_t new_size);

        u32_t get_size();

        byte_t read(u32_t at);

        void write(u32_t at, byte_t b);
    };
}
