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

        void assertSafe() {
            if (safeMode && (pos + 1) > maxSafePos) {
                scream("Reader safe mode boundaries violation");
            }
        }

        void assertSafe(pos_t length) {
            if (safeMode && (pos + length) > maxSafePos) {
                scream("Reader safe mode boundaries violation");
            }
        }

    public:
        Reader(const content_t& c):
            data(c),
            size(c.size()),
            pos(0),
            maxSafePos(0)
        {}

        bool eof() {
            return pos >= size;
        }

        void setSafeUntil(pos_t pos_) {
            safeMode = true;
            maxSafePos = pos_;
        }

        pos_t getPos() {
            return pos;
        }

        byte_t next() {
            assertSafe();

            return data[pos++];
        }

        void seekTo(pos_t p) {
            safeMode = false;
            pos = p;
        }

        content_t content(int length) {
            assertSafe(length);

            auto from = data.begin() + pos;
            auto to = from + length;

            pos += length;

            return {from, to};
        }
    };
}
