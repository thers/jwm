#pragma once

#include <exception>
#include "wasm.h"

namespace jwm {
    class Trap : std::exception {
        u32_t pos;
    public:
        Trap(u32_t at) : pos(at) {}

        virtual const char* what() {
            return "Trap occurred";
        }
    };

    static const Trap newTrap(u32_t pos) {
        return pos;
    }
}
