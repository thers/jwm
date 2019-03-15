#pragma once

#include <runtime/types.h>
#include <runtime/Module.h>
#include <runtime/Store.h>
#include <runtime/Stack.h>
#include <runtime/Env.h>
#include <runtime/jit/jit.h>
#include <runtime/baseline/baseline.h>

namespace runtime {
    Module decode_module(content_t &content);

    void parse_module(std::string& content);
}
