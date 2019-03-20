#pragma once

#include <tuple>
#include <optional>

#include <jwm/stdinc.h>
#include <jwm/wasm.h>

#include "reader.h"

namespace jwm::wasm::decoders {
    template <typename T>
    T signedInteger(Reader& reader) {
        T result = 0;
        int shift = 0;
        byte_t byte;

        do {
            byte = reader.next();

            if (reader.eof()) {
                return result;
            }

            result |= (static_cast<T>(byte & 0x7f)) << shift;

            shift += 7;
        } while (byte & 0x80);

        if ((shift < (8 * sizeof(T))) && (byte & 0x40)) {
            result |= (static_cast<T>(~0)) << shift;
        }

        return result;
    }

    inline byte_t byte(Reader& reader) {
        return reader.next();
    }

    template <typename T>
    T reinterpretBytes(Reader& reader) {
        auto bytes = reader.content(sizeof(T));

        return *reinterpret_cast<T *>(bytes.data());
    }

    template <typename T>
    T byteEnumItem(Reader& reader) {
        return static_cast<T>(byte(reader));
    }

    inline u32_t u32(Reader& reader) {
        u32_t result = 0;
        u32_t shift = 0;
        byte_t byte;

        do {
            byte = reader.next();

            result |= static_cast<u32_t>(byte & 0x7f) << shift;

            shift += 7;
        } while (byte & 0x80);

        return result;
    }

    inline i32_t i32(Reader& reader) {
        return signedInteger<i32_t>(reader);
    }

    inline i64_t i64(Reader& reader) {
        return signedInteger<i64_t>(reader);
    }

    inline f32_t f32(Reader& reader) {
        return reinterpretBytes<f32_t>(reader);
    }

    inline f64_t f64(Reader& reader) {
        return reinterpretBytes<f64_t>(reader);
    }

    template <typename T, typename F>
    inline vec_t<T> vec(Reader& reader, F readVal) {
        u32_t length = u32(reader);

        vec_t<T> result;
        result.reserve(length);

        for (std::size_t index = 0; index < length; index++) {
            result.push_back(readVal(reader));
        }

        return result;
    }

    template <typename T, typename F>
    inline void vec2(T *result, Reader& reader, F readVal) {
        u32_t length = u32(reader);
        result->reserve(length);

        for (std::size_t index = 0; index < length; index++) {
            result->push_back(readVal(reader));
        }
    }

    inline content_t bytesVec(Reader& reader) {
        return vec<byte_t>(reader, byte);
    }

    inline memory_immediate_arg_decl_t memop_args(Reader& reader) {
        return {u32(reader), u32(reader)};
    }

    inline func_args_decl_t functype(Reader& reader) {
        auto funcbyte = byteEnumItem<type>(reader);

        if (funcbyte != type::t_functype) {
            scream("\nAttempt to parse functype that isn't a functype\n");
        }

        auto parameterTypes = vec<valtype>(reader, byteEnumItem<valtype>);
        auto resultTypes = vec<valtype>(reader, byteEnumItem<valtype>);

        return {parameterTypes, resultTypes};
    }

    inline name_t name(Reader& reader) {
        auto bytes = vec<byte_t>(reader, byte);

        return {bytes.begin(), bytes.end()};
    }

    inline limit_decl_t limit(Reader& reader) {
        auto type = byteEnumItem<limittype>(reader);

        if (type == limittype::mt_finite) {
            return {true, u32(reader), u32(reader)};
        }

        return {false, u32(reader), 0};
    }

    inline table_decl_t table(Reader &reader) {
        auto elemtype = byte(reader);

        if (elemtype != 0x70) {
            scream("\nWrong table_decl_t\n");
        }

        return limit(reader);
    }

    inline mem_decl_t mem(Reader &reader) {
        return limit(reader);
    }

    inline global_type_decl_t global(Reader &reader) {
        auto valtype = byteEnumItem<wasm::valtype>(reader);
        auto mut = byteEnumItem<wasm::mut>(reader);

        return {valtype, mut == mut::m_var};
    }

    inline section_decl_t section(Reader& reader) {
        auto type = byteEnumItem<wasm::section>(reader);
        auto size = u32(reader);

        return {type, size};
    }

    inline result_decl_t result(Reader& reader) {
        return {byteEnumItem<wasm::valtype>(reader)};
    }

    inline br_table_arg_t br_table_arg(Reader& reader) {
        auto labels = decoders::vec<labelidx_decl_t>(reader, u32);
        labelidx_decl_t labelidx = decoders::u32(reader);

        return {labels, labelidx};
    }

    inline locals_decl_t local(Reader& reader) {
        return {u32(reader), byteEnumItem<valtype>(reader)};
    }

    inline vec_t<locals_decl_t> locals(Reader& reader) {
        return vec<locals_decl_t>(reader, local);
    }

    static constexpr_decl_t constexprd(Reader& reader) {
        constexpr_decl_t constexpression;

        auto op = byteEnumItem<constinstrtype>(reader);
        constinstr_arg_decl_t arg;

        while (op != constinstrtype::cit_end) {
            switch (op) {
                case constinstrtype::cit_i32:
                    arg = i32(reader);
                    break;
                case constinstrtype::cit_i64:
                    arg = i64(reader);
                    break;
                case constinstrtype::cit_f32:
                    arg = f32(reader);
                    break;
                case constinstrtype::cit_f64:
                    arg = f64(reader);
                    break;

                case constinstrtype::cit_global_get:
                    arg = constinstr_arg_decl_t {u32(reader)};
                    break;

                default:
                    scream("Unknown constexpr opcode");
                    break;
            }

            constexpression.push_back({op, arg});

            op = decoders::byteEnumItem<constinstrtype>(reader);
        }

        return constexpression;
    }

    static expr_decl_t expr(Reader& reader) {
        expr_decl_t expression;

        auto op = byteEnumItem<opcode>(reader);
        auto opname = opcode_names[op];
        instr_arg_decl_t arg;

        while (op != opcode::op_end) {
            if (!opname) {
                scream("Unknown opcode");
            }

            switch (op) {
                case opcode::op_br:
                case opcode::op_br_if:

                case opcode::op_call:
                case opcode::op_call_indirect:

                case opcode::op_tee_local:

                case opcode::op_get_local:
                case opcode::op_set_local:

                case opcode::op_get_global:
                case opcode::op_set_global:
                    arg = instr_arg_decl_t {u32(reader)};
                    break;


                case opcode::op_f32_load:
                case opcode::op_f32_store:

                case opcode::op_f64_load:
                case opcode::op_f64_store:

                case opcode::op_i32_load:
                case opcode::op_i32_load_8_s:
                case opcode::op_i32_load_8_u:
                case opcode::op_i32_load_16_s:
                case opcode::op_i32_load_16_u:
                case opcode::op_i32_store:
                case opcode::op_i32_store_8:
                case opcode::op_i32_store_16:

                case opcode::op_i64_load:
                case opcode::op_i64_load_8_s:
                case opcode::op_i64_load_8_u:
                case opcode::op_i64_load_16_s:
                case opcode::op_i64_load_16_u:
                case opcode::op_i64_load_32_s:
                case opcode::op_i64_load_32_u:
                case opcode::op_i64_store:
                case opcode::op_i64_store_8:
                case opcode::op_i64_store_16:
                case opcode::op_i64_store_32:
                    arg = memop_args(reader);
                    break;


                case opcode::op_i32_const:
                    arg = i32(reader);
                    break;
                case opcode::op_i64_const:
                    arg = i64(reader);
                    break;
                case opcode::op_f32_const:
                    arg = f32(reader);
                    break;
                case opcode::op_f64_const:
                    arg = f64(reader);
                    break;


                case opcode::op_block:
                case opcode::op_loop:
                case opcode::op_if:
                    arg = result(reader);
                    break;


                case opcode::op_br_table:
                    arg = br_table_arg(reader);
                    break;


                default:
                    arg = std::nullopt;
                    break;
            }

            expression.push_back({op, opname, arg});

            op = byteEnumItem<opcode>(reader);
            opname = opcode_names[op];
        }

        return expression;
    }

    inline func_decl_t func(Reader& reader) {
        return {locals(reader), expr(reader)};
    }

    inline code_decl_t code(Reader& reader) {
        return {u32(reader), func(reader)};
    }

    inline element_decl_t element(Reader& reader) {
        auto table = u32(reader);
        auto offset = constexprd(reader);
        auto init = vec<index_decl_t>(reader, u32);

        return {table, offset, init};
    }

    inline global_decl_t globaldesc(Reader& reader) {
        auto type = global(reader);
        auto init = constexprd(reader);

        return {type, init};
    }

    inline export_decl_t exportdesc(Reader& reader) {
        auto nm = name(reader);
        auto type = byteEnumItem<exporttype>(reader);
        auto idx = u32(reader);

        return {nm, type, idx};
    }

    inline import_decl_t importdesc(Reader& reader) {
        auto module = name(reader);
        auto name_ = name(reader);

        auto type = byteEnumItem<importtype>(reader);
        import_decl_val_t val;

        switch (type) {
            case importtype::it_typeidx:
                val = u32(reader);
                break;

            case importtype::it_tabletype:
                val = table(reader);
                break;

            case importtype::it_memtype:
                val = mem(reader);
                break;

            case importtype::it_globaltype:
                val = global(reader);
                break;
        }

        return {module, name_, type, val};
    }

    inline data_t data(Reader& reader) {
        auto data = u32(reader);
        auto offset = constexprd(reader);
        auto init = bytesVec(reader);

        return {data, offset, init};
    }
}
