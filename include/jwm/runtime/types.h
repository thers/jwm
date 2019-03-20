#pragma once

#include <jwm/wasm.h>

namespace jwm::runtime {
    using trap_t = void (*)();

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}
