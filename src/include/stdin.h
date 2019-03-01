#pragma once

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <memory>
#include <string_view>
#include <stdexcept>
#include <string>

typedef uint8_t byte_t;

inline auto print = std::printf;

inline void scream(std::string_view msg) {
    throw std::runtime_error {msg.data()};
}

inline void printb(byte_t byte) {
    print("%02X ", byte);
}

inline void printbv(std::vector<byte_t> dat) {
    int i = 0;

    for (auto ch: dat) {
        printb(ch);

        i++;

        if (i % 10 == 0) {
            print("\n");
        }
    }
}
