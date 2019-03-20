#pragma once

#include "stdinc.h"
#include "wasm.h"
#include "runtime.h"

namespace jwm {
    runtime::Module decode_module(content_t &content);
}
