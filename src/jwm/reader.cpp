#include "reader.h"

namespace jwm::wasm {
    void Reader::assertSafe() {
        if (safeMode && (pos + 1) > maxSafePos) {
            scream("Reader safe mode boundaries violation");
        }
    }

    void Reader::assertSafe(pos_t length) {
        if (safeMode && (pos + length) > maxSafePos) {
            scream("Reader safe mode boundaries violation");
        }
    }

    bool Reader::eof() {
        return pos >= size;
    }

    void Reader::setSafeUntil(pos_t pos_) {
        safeMode = true;
        maxSafePos = pos_;
    }

    pos_t Reader::getPos() {
        return pos;
    }

    byte_t Reader::next() {
        assertSafe();

        return data[pos++];
    }

    void Reader::seekTo(pos_t p) {
        safeMode = false;
        pos = p;
    }

    content_t Reader::content(int length) {
        assertSafe(length);

        auto from = data.begin() + pos;
        auto to = from + length;

        pos += length;

        return {from, to};
    }
}
