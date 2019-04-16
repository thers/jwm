#pragma once

#include <map>
#include <string>
#include <variant>

#include "wasm.h"
#include "runtime.h"

namespace jwm {
    using HostFunction = std::optional<val_t> (*)(std::optional<vec_t<val_t>>);

    using ModuleImportsValue = std::variant<u32_t, u64_t, i32_t, i64_t, f32_t, f64_t, HostFunction>;
    using ModuleImports = std::map<std::string, std::map<std::string, ModuleImportsValue>>;
}
