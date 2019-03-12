#pragma once

#include <stdin.h>
#include <wasm.h>
#include <tuple>
#include <algorithm>

namespace wasm::decoders {
    template <typename T>
    T unsignedInteger(Reader& reader) {
        T result = 0;
        int shift = 0;
        byte_t byte;

        do {
            byte = reader.next();

            result |= (T)(byte & 0x7f) << shift;

            shift += 7;
        } while (byte & 0x80);

        return result;
    }

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
        std::reverse(std::begin(bytes), std::end(bytes));

        return *reinterpret_cast<T *>(bytes.data());
    }

    template <typename T>
    T byteEnumItem(Reader& reader) {
        return static_cast<T>(byte(reader));
    }

    inline u32_t u32(Reader& reader) {
        return unsignedInteger<u32_t>(reader);
    }

    inline u64_t u64(Reader& reader) {
        return unsignedInteger<u64_t>(reader);
    }

    inline i32_t i32(Reader& reader) {
        return signedInteger<i32_t>(reader);
    }

    inline i64_t i64(Reader& reader) {
        return signedInteger<i64_t>(reader);
    }

    inline f32_t f32(Reader& reader) {
        auto bytes = reader.content(sizeof(f32_t));

        return *reinterpret_cast<f32_t *>(bytes.data());
    }

    inline f64_t f64(Reader& reader) {
        auto bytes = reader.content(sizeof(f64_t));

        return *reinterpret_cast<f64_t *>(bytes.data());
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

    inline auto readByte = [] (Reader& reader) {
        return u32(reader);
    };

    inline memop_arg_t memop_args(Reader& reader) {
        return {u32(reader), u32(reader)};
    }

    inline functype_t functype(Reader& reader) {
        auto funcbyte = byteEnumItem<type>(reader);

        if (funcbyte != type::t_functype) {
            scream("\nAttempt to parse functype that isn't a functype\n");
        }

        auto readValtype = [] (Reader& reader) {
            return byteEnumItem<valtype>(reader);
        };

        auto parameterTypes = vec<valtype>(reader, readValtype);
        auto resultTypes = vec<valtype>(reader, readValtype);

        return {parameterTypes, resultTypes};
    }

    inline name_t name(Reader& reader) {
        auto readByte = [] (Reader& reader) {
            return byte(reader);
        };

        auto bytes = vec<byte_t>(reader, readByte);

        return {bytes.begin(), bytes.end()};
    }

    inline limit_t limit(Reader& reader) {
        auto type = byteEnumItem<wasm::limittype>(reader);

        if (type == limittype::mt_finite) {
            return {true, u32(reader), u32(reader)};
        }

        return {false, u32(reader), 0};
    }

    inline table_t table(Reader &reader) {
        auto elemtype = byte(reader);

        if (elemtype != 0x70) {
            scream("\nWrong table_t\n");
        }

        return limit(reader);
    }

    inline mem_t mem(Reader &reader) {
        return limit(reader);
    }

    inline global_t global(Reader &reader) {
        auto valtype = byteEnumItem<wasm::valtype>(reader);
        auto mut = byteEnumItem<wasm::mut>(reader);

        return {valtype, mut == mut::m_const};
    }

    inline section_t section(Reader& reader) {
        auto type = byteEnumItem<wasm::section>(reader);
        auto size = u32(reader);

        return {type, size};
    }

    inline result_t result(Reader& reader) {
        return {byteEnumItem<wasm::valtype>(reader)};
    }

    inline br_table_arg_t br_table_arg(Reader& reader) {
        auto readLabelidx = [] (Reader& reader) {
            return decoders::u32(reader);
        };

        auto labels = decoders::vec<labelidx_t>(reader, readLabelidx);
        labelidx_t labelidx = decoders::u32(reader);

        return {labels, labelidx};
    }

    inline local_t local(Reader& reader) {
        return {u32(reader), byteEnumItem<valtype>(reader)};
    }

    inline vec_t<local_t> locals(Reader& reader) {
        auto readLocal = [] (Reader& reader) {
            return local(reader);
        };

        return vec<local_t>(reader, readLocal);
    }

    static constexpr_t constexprd(Reader& reader) {
        constexpr_t constexpression;

        auto op = decoders::byteEnumItem<constinstrtype>(reader);
        constinstr_arg_t arg;

        while (op != constinstrtype::cit_end) {
            switch (op) {
                case constinstrtype::cit_i32:
                    arg = decoders::i32(reader);
                    break;
                case constinstrtype::cit_i64:
                    arg = decoders::i64(reader);
                    break;
                case constinstrtype::cit_f32:
                    arg = decoders::f32(reader);
                    break;
                case constinstrtype::cit_f64:
                    arg = decoders::f64(reader);
                    break;

                case constinstrtype::cit_global_get:
                    arg = constinstr_arg_t {decoders::u32(reader)};
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

    static expr_t expr(Reader& reader) {
        expr_t expression;

        auto op = decoders::byteEnumItem<opcode>(reader);
        auto opname = opcode_names[op];
        instr_arg_t arg;

        while (op != opcode::op_end) {
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
                    arg = instr_arg_t {decoders::u32(reader)};
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
                    arg = decoders::memop_args(reader);
                    break;


                case opcode::op_i32_const:
                    arg = decoders::i32(reader);
                    break;
                case opcode::op_i64_const:
                    arg = decoders::i64(reader);
                    break;
                case opcode::op_f32_const:
                    arg = decoders::f32(reader);
                    break;
                case opcode::op_f64_const:
                    arg = decoders::f64(reader);
                    break;


                case opcode::op_block:
                case opcode::op_loop:
                case opcode::op_if:
                    arg = decoders::result(reader);
                    break;


                case opcode::op_br_table:
                    arg = decoders::br_table_arg(reader);
                    break;


                default:
                    arg = NULL;
                    break;
            }

            expression.push_back({op, opname, arg});

            op = decoders::byteEnumItem<opcode>(reader);
            opname = opcode_names[op];
        }

        return expression;
    }

    inline func_t func(Reader& reader) {
        return {locals(reader), expr(reader)};
    }

    inline code_t code(Reader& reader) {
        return {u32(reader), func(reader)};
    }

    inline element_t element(Reader& reader) {
        auto table = u32(reader);
        auto offset = constexprd(reader);
        auto init = vec<index_t>(reader, readByte);

        return {table, offset, init};
    }
}
