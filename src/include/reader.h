#pragma once

#include <stdin.h>
#include <wasm.h>

typedef std::size_t pos_t;

class Reader {
private:
    const wasm::content_t& data;
    pos_t size;
    pos_t pos;

public:
    Reader(const wasm::content_t& c): data(c), size(c.size()), pos(0) {}

    bool eof() {
        return pos >= size;
    }

    pos_t get_pos() {
        return pos;
    }

    byte_t next() {
        return data[pos++];
    }

    wasm::content_t content(int length) {
        auto from = data.begin() + pos;
        auto to = from + length;

        pos += length;

        return {from, to};
    }
};
