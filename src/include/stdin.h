#pragma once

#include <iostream>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <memory>
#include <string_view>

typedef uint8_t byte_t;
typedef std::vector<byte_t> module_t;

inline auto print = std::printf;

inline void scream(const char* msg, int code = 0) {
    std::cerr << msg;
    throw code;
}

inline void printb(uint8_t byte) {
    print("%02X ", byte);
}

inline void printv(std::vector<uint8_t> dat) {
    int i = 0;

    for (auto ch: dat) {
        printb(ch);

        i++;

        if (i % 10 == 0) {
            print("\n");
        }
    }
}
