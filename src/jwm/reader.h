#pragma once

#include <jwm/wasm.h>

namespace jwm::wasm {
    class Reader {
    private:
        const content_t data;
        pos_t size;
        pos_t pos;

        bool safeMode = false;
        pos_t maxSafePos;

        void assertSafe();

        void assertSafe(pos_t length);

    public:
        Reader(const content_t& c):
                data(c),
                size(c.size()),
                pos(0),
                maxSafePos(0)
        {}

        bool eof();

        void setSafeUntil(pos_t pos_);

        pos_t getPos();

        byte_t next();

        void seekTo(pos_t p);

        content_t content(int length);
    };
}
