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

            result |= (byte & 0x7f) << shift;

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

            result |= (byte & 0x7f) << shift;

            shift += 7;
        } while (byte & 0x80);

        if ((shift < (8 * sizeof(result))) && (byte & 0x40)) {
            result |= (-1) << shift;
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

    inline wasm::u32_t u32(Reader& reader) {
        return unsignedInteger<wasm::u32_t>(reader);
    }

    inline wasm::u64_t u64(Reader& reader) {
        return unsignedInteger<wasm::u64_t>(reader);
    }

    inline wasm::i32_t i32(Reader& reader) {
        return signedInteger<wasm::i32_t>(reader);
    }

    inline wasm::i64_t i64(Reader& reader) {
        return signedInteger<wasm::i64_t>(reader);
    }

    template <typename T, typename F>
    inline wasm::vec_t<T> vec(Reader& reader, F readVal) {
        wasm::u32_t length = u32(reader);

        wasm::vec_t<T> result(length);

        for (std::size_t index = 0; index < length; index++) {
            result[index] = readVal();
        }

        return result;
    }

    inline wasm::memop_t memory(wasm::opcode op, Reader& reader) {
        return {op, {u32(reader), u32(reader)}};
    }

    inline wasm::func_t functype(Reader& reader) {
        auto funcbyte = byteEnumItem<wasm::type>(reader);

        if (funcbyte != wasm::type::t_functype) {
            scream("\nAttempt to parse functype that isn't a functype\n");
        }

        auto readValtype = [&] () {
            return byteEnumItem<wasm::valtype>(reader);
        };

        auto parameterTypes = vec<wasm::valtype>(reader, readValtype);
        auto resultTypes = vec<wasm::valtype>(reader, readValtype);

        return {parameterTypes, resultTypes};
    }

    inline wasm::name_t name(Reader& reader) {
        auto readByte = [&] () {
            return byte(reader);
        };

        auto bytes = vec<byte_t>(reader, readByte);

        return {bytes.begin(), bytes.end()};
    }

    inline wasm::limit_t limit(Reader& reader) {
        auto limittype = byteEnumItem<wasm::limittype>(reader);

        if (limittype == wasm::limittype::mt_finite) {
            return {true, u32(reader), u32(reader)};
        }

        return {false, u32(reader), 0};
    }

    inline wasm::table_t table(Reader &reader) {
        auto elemtype = byte(reader);

        if (elemtype != 0x70) {
            scream("\nWrong table_t\n");
        }

        return limit(reader);
    }

    inline wasm::mem_t mem(Reader &reader) {
        return limit(reader);
    }

    inline wasm::global_t global(Reader &reader) {
        auto valtype = byteEnumItem<wasm::valtype>(reader);
        auto mut = byteEnumItem<wasm::mut>(reader);

        return {valtype, mut == wasm::mut::m_const};
    }

    inline wasm::section_t section(Reader& reader) {
        auto type = byteEnumItem<wasm::section>(reader);
        auto size = u32(reader);

        return {type, size};
    }
}
