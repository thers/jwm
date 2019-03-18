#pragma once

#include <wasm.h>

namespace runtime {
    using trap_t = void (*)();

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}
