#pragma once

#include <stdin.h>

namespace wasm {
    enum type : byte_t {
        t_i32 = 0x7f,
        t_i64 = 0x7e,
        t_f32 = 0x7d,
        t_f64 = 0x7c,

        t_funcref = 0x70,

        t_functype = 0x60,

        t_result = 0x40,
    };

    enum valtype : byte_t {
        vt_i32 = type::t_i32,
        vt_i64 = type::t_i64,
        vt_f32 = type::t_f32,
        vt_f64 = type::t_f64,
    };

    enum importtype : byte_t {
        it_typeidx = 0x00,
        it_tabletype = 0x01,
        it_memtype = 0x02,
        it_globaltype = 0x03,
    };

    enum blocktype : byte_t {
        bt_empty = 0x40
    };

    enum mut : byte_t {
        m_const = 0x00,
        m_var = 0x01
    };

    enum limittype : byte_t {
        mt_infinite = 0x00,
        mt_finite = 0x01,
    };
}
