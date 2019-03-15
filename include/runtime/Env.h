#pragma once

#include <runtime/types.h>
#include <runtime/Module.h>
#include <runtime/Store.h>
#include <runtime/Stack.h>

namespace runtime {
    class Env {
        Store store;
        Stack stack;
        Module module;


    };
}
