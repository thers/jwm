#pragma once

#include <stdin.h>
#include <parser.h>
#include <wasm.h>
#include <tuple>

namespace wasm::decoders {
    template <typename T, typename F>
    inline wasm::vec<T> vec(Parser& parser, F readVal) {
        u32 length = parser.read_u32();

        wasm::vec<T> result(length);

        std::size_t index = 0;

        for (; index < length; index++) {
            result[index] = readVal();
        }

        return result;
    }

    inline wasm::memop memory(wasm::opcode op, Parser& parser) {
        return {op, {parser.read_u32(), parser.read_u32()}};
    }

    inline wasm::funcdecl functype(Parser& parser) {
        byte_t funcbyte = parser.read();

        if (funcbyte != wasm::type::t_functype) {
            scream("\nAttempt to parse functype that isn't a functype\n");
        }

        auto readValtype = [&] () { return static_cast<wasm::valtype>(parser.read()); };

        auto parameterTypes = vec<wasm::valtype>(parser, readValtype);
        auto resultTypes = vec<wasm::valtype>(parser, readValtype);

        return {parameterTypes, resultTypes};
    }

    inline wasm::name name(Parser& parser) {
        auto readByte = [&] () {
            return parser.read();
        };

        auto bytes = vec<byte_t>(parser, readByte);

        return {bytes.begin(), bytes.end()};
    }

    inline wasm::limit_finite limit_finite(Parser& parser) {
        return {parser.read_u32(), parser.read_u32()};
    }

    inline wasm::tabletype tabletype(Parser& parser) {
        auto elemtype = parser.read();

        if (elemtype != 0x70) {
            scream("\nWrong tabletype\n");
        }

        return limit_finite(parser);
    }

    inline wasm::memtype memtype(Parser& parser) {
        return limit_finite(parser);
    }

    inline wasm::globaltype globaltype(Parser& parser) {
        auto t = static_cast<wasm::valtype>(parser.read());
        auto m = static_cast<wasm::mut>(parser.read());

        return {t, m};
    }
}
