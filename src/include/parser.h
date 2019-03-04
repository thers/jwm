#pragma once

#include <stdin.h>
#include <wasm.h>
#include <vector>
#include <algorithm>

typedef std::size_t pos_t;

class Parser {
private:
    const wasm::content_t& data;
    pos_t size;
    pos_t pos;

    template <typename T>
    T readUnsigned() {
        T result = 0;
        int shift = 0;
        uint8_t byte;

        do {
            byte = this->read();

            result |= (byte & 0x7f) << shift;

            shift += 7;
        } while (byte & 0x80);

        return result;
    }

    template <typename T>
    T readSigned() {
        T result = 0;
        int shift = 0;
        uint8_t byte;

        do {
            byte = this->read();

            result |= (byte & 0x7f) << shift;

            shift += 7;
        } while (byte & 0x80);

        if ((shift < (8 * sizeof(result))) && (byte & 0x40)) {
            result |= (-1) << shift;
        }

        return result;
    }

public:
    Parser(const wasm::content_t& c): data(c), size(c.size()), pos(0) {}

    bool eof() {
        return pos >= size;
    }

    pos_t get_pos() {
        return pos;
    }

    byte_t read() {
        return data[pos++];
    }

    wasm::content_t read(int length) {
        auto from = data.begin() + pos;
        auto to = from + length;

        pos += length;

        return {from, to};
    }

    template <typename T>
    T read() {
        auto bytes = read(sizeof(T));
        std::reverse(std::begin(bytes), std::end(bytes));

        return *reinterpret_cast<T *>(bytes.data());
    }

    template <typename T>
    T read_type() {
        return static_cast<T>(read());
    }

    wasm::u32 read_u32() {
        return readUnsigned<wasm::u32>();
    }

    wasm::i32 read_i32() {
        return readSigned<wasm::i32>();
    }

    wasm::u64 read_u64() {
        return readUnsigned<wasm::u64>();
    }

    wasm::i64 read_i64() {
        return readSigned<wasm::i64>();
    }
};
