#pragma once

#include <runtime/types.h>
#include <runtime/Module.h>
#include <runtime/Store.h>
#include <runtime/Stack.h>
#include <runtime/Env.h>
#include <runtime/Executor.h>

namespace runtime {
    Module decode_module(content_t &content);

    void parse_module(std::string& content);
}
