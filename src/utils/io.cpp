#include "io.h"

namespace jwmio {
    std::vector<uint8_t> readModule(std::string path) {
        std::ifstream file(path, std::ios::in|std::ios::binary);
        file.unsetf(std::ios::skipws);

        std::istream_iterator<uint8_t> begin(file), end;

        return std::vector<uint8_t> (begin, end);
    }
}
