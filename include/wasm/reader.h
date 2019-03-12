#pragma once

#include "types.h"

namespace wasm {
    class Reader {
    private:
        const content_t data;
        pos_t size;
        pos_t pos;

        bool safeMode = false;
        pos_t maxSafePos;

        void checkIfSafe() {
            if (safeMode && (pos + 1) > maxSafePos) {
                scream("Reader safe mode boundaries violation");
            }
        }

        void checkIfSafe(pos_t length) {
            if (safeMode && (pos + length) > maxSafePos) {
                scream("Reader safe mode boundaries violation");
            }
        }

    public:
        Reader(const content_t c): data(c), size(c.size()), pos(0) {}

        bool eof() {
            return pos >= size;
        }

        void safe_until(pos_t pos) {
            safeMode = true;
            maxSafePos = pos;
        }

        pos_t get_pos() {
            return pos;
        }

        byte_t next() {
            checkIfSafe();

            return data[pos++];
        }

        void seek_to(pos_t p) {
            safeMode = false;
            pos = p;
        }

        content_t content(int length) {
            checkIfSafe(length);

            auto from = data.begin() + pos;
            auto to = from + length;

            pos += length;

            return {from, to};
        }
    };
}
