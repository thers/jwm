#pragma once

#include <stdin.h>
#include <reader.h>
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

    inline wasm::u32 u32(Reader& reader) {
        return unsignedInteger<wasm::u32>(reader);
    }

    inline wasm::u64 u64(Reader& reader) {
        return unsignedInteger<wasm::u64>(reader);
    }

    inline wasm::i32 i32(Reader& reader) {
        return signedInteger<wasm::i32>(reader);
    }

    inline wasm::i64 i64(Reader& reader) {
        return signedInteger<wasm::i64>(reader);
    }

    template <typename T, typename F>
    inline wasm::vec<T> vec(Reader& reader, F readVal) {
        wasm::u32 length = u32(reader);

        wasm::vec<T> result(length);

        std::size_t index = 0;

        for (; index < length; index++) {
            result[index] = readVal();
        }

        return result;
    }

    inline wasm::memop memory(wasm::opcode op, Reader& reader) {
        return {op, {u32(reader), u32(reader)}};
    }

    inline wasm::funcdecl functype(Reader& reader) {
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

    inline wasm::name name(Reader& reader) {
        auto readByte = [&] () {
            return byte(reader);
        };

        auto bytes = vec<byte_t>(reader, readByte);

        return {bytes.begin(), bytes.end()};
    }

    inline wasm::limit_finite limit_finite(Reader& reader) {
        return {u32(reader), u32(reader)};
    }

    inline wasm::tabletype tabletype(Reader& reader) {
        auto elemtype = byte(reader);

        if (elemtype != 0x70) {
            scream("\nWrong tabletype\n");
        }

        return limit_finite(reader);
    }

    inline wasm::memtype memtype(Reader& reader) {
        return limit_finite(reader);
    }

    inline wasm::globaltype globaltype(Reader& reader) {
        auto t = byteEnumItem<wasm::valtype>(reader);
        auto m = byteEnumItem<wasm::mut>(reader);

        return {t, m};
    }
}
