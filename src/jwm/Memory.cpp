#include <jwm/jwm.h>

using namespace jwm::wasm;

namespace jwm::runtime {
    void Memory::check_boundaries(u32_t at) {
        if (at < 0 || at >= max_offset) {
            trap();
        }
    }

    void Memory::grow(u32_t new_size) {
        max_offset = new_size * page_size;
        m_ptr = reinterpret_cast<byte_t*>(std::realloc(m_ptr, max_offset));
    }

    u32_t Memory::get_size() {
        return size;
    }

    byte_t Memory::read(u32_t at) {
        check_boundaries(at);

        return m_ptr[at];
    }

    vec_t<byte_t> Memory::read(u32_t at, size_t length) {
        check_boundaries(at);
        check_boundaries(at + length);

        vec_t<byte_t> target;
        target.resize(length);

        std::memcpy(&target[0], &m_ptr[at], length);

        return target;
    }

    void Memory::write(u32_t at, byte_t b) {
        check_boundaries(at);

        m_ptr[at] = b;
    }

    void Memory::write(u32_t at, vec_t<byte_t> b) {
        check_boundaries(at);
        check_boundaries(at + b.size());

        std::memcpy(&m_ptr[at], b.data(), b.size());
    }
}
