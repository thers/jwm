#pragma once

#include <cstdint>
#include <vector>
#include <memory>
#include <string_view>
#include <stdexcept>

#include <fmt/format.h>

namespace jwm {
    typedef uint8_t byte_t;

    template <typename... T>
    void print(std::string_view msg, T... args) {
        fmt::print(msg, args...);
    }

    template <typename... T>
    void printl(std::string_view msg, T... args) {
        fmt::print(msg, args...);
        fmt::print("\n");
    }

    template <typename... T>
    inline void scream(std::string_view msg, T... args) {
        throw std::runtime_error {fmt::format(msg, args...)};
    }

    inline void printb(byte_t byte) {
        print("{:02x} ", byte);
    }

    inline void printbv(std::vector<byte_t>& dat) {
        int i = 0;

        for (auto ch: dat) {
            printb(ch);

            i++;

            if (i % 10 == 0) {
                print("\n");
            }
        }

        print("\n");
    }
}
