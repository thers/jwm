#include <jwm/stdinc.h>
#include <jwm/runtime.h>

namespace jwm::runtime::executor {
    template <typename T>
    inline T popOne(operands_stack_t &stack) {
        auto p1 = stack.top();
        stack.pop();
        return std::get<T>(p1);
    }

    template <typename T>
    inline std::pair<T, T> popTwo(operands_stack_t &stack) {
        return {popOne<T>(stack), popOne<T>(stack)};
    }

    val_t initVal(valtype type) {
        switch (type) {
            default:
            case valtype::vt_empty:
            case valtype::vt_i32:
            case valtype::vt_i64:
                return 0;

            case valtype::vt_f32:
            case valtype::vt_f64:
                return 0.0;
        }
    }

    val_t reduceInstrArgToVal(instr_arg_decl_t arg) {
        if (!arg.has_value()) {
            scream("Can not reduce empty op operand to value type");
        }

        auto argVariant = arg.value();

        if (std::holds_alternative<i32_t>(argVariant)) {
            return std::get<i32_t>(argVariant);
        }
        if (std::holds_alternative<i64_t>(argVariant)) {
            return std::get<i64_t>(argVariant);
        }
        if (std::holds_alternative<f32_t>(argVariant)) {
            return std::get<f32_t>(argVariant);
        }
        if (std::holds_alternative<f64_t>(argVariant)) {
            return std::get<f64_t>(argVariant);
        }

        scream("Can not reduce op operand to value type");
    }

    val_t constexprEval(runtime::Store &store, ModuleInst &moduleInst, const constexpr_decl_t &expr) {
        operands_stack_t stack;

        for (auto[op, arg]: expr) {
            switch (op) {
                case constinstrtype::cit_i32:
                    stack.push(std::get<i32_t>(arg));
                    break;

                case constinstrtype::cit_i64:
                    stack.push(std::get<i64_t>(arg));
                    break;

                case constinstrtype::cit_f32:
                    stack.push(std::get<f32_t>(arg));
                    break;

                case constinstrtype::cit_f64:
                    stack.push(std::get<f64_t>(arg));
                    break;

                case constinstrtype::cit_global_get: {
                    auto global = store.get_global(moduleInst, std::get<index_decl_t>(arg));

                    std::visit(overloaded{
                            [&](auto v) { scream("Error during global initialization"); },
                            [&](i32_t v) { stack.push(v); },
                            [&](i64_t v) { stack.push(v); },
                            [&](f32_t v) { stack.push(v); },
                            [&](f64_t v) { stack.push(v); },
                    }, global);

                    break;
                }

                case constinstrtype::cit_end:
                    scream("Unexpected end op");
                    break;

                default:
                    scream("Unexpected constexpr op");
                    break;
            }
        }

        if (stack.empty()) {
            scream("Invalid constexpr initialization sequence, missing end op");
        }

        return stack.top();
    }

    val_t functionEval(
            runtime::Store &store,
            ModuleInst &moduleInst,
            const func_decl_t &func,
            const args_t &args
    ) {
        vec_t<val_t> locals;
        operands_stack_t stack;

        auto push = [&](val_t val) {
            stack.push(val);
        };

        auto pushArg = [&](instr_arg_decl_t arg) {
            stack.push(reduceInstrArgToVal(arg));
        };

        auto pop = [&]() {
            val_t val = stack.top();
            stack.pop();
            return val;
        };

        auto peek = [&]() {
            return stack.top();
        };

        auto getLocal = [&](instr_arg_decl_t arg) {
            auto index = std::get<index_decl_t>(arg.value());
            return locals[index];
        };

        auto setLocal = [&](instr_arg_decl_t arg, val_t val) {
            auto index = std::get<index_decl_t>(arg.value());
            locals[index] = val;
        };

        for (auto arg: args) {
            locals.push_back(arg);
        }

        for (auto local: func.locals) {
            for (size_t i = 0; i < local.num_locals; i++) {
                locals.push_back(initVal(local.type));
            }
        }

        for (auto[op, opname, arg]: func.exp) {
            switch (op) {
                case opcode::op_call: {
                    auto funcIndex = std::get<index_decl_t>(arg.value());
                    auto func = store.get_func(moduleInst, funcIndex);

                    auto funcType = func.get_type();

                    args_t args;

                    for (auto argType: funcType.arguments_types) {
                        switch (argType) {
                            case valtype::vt_i32: {
                                args.push_back(popOne<i32_t>(stack));
                                break;
                            }
                            case valtype::vt_i64: {
                                args.push_back(popOne<i64_t>(stack));
                                break;
                            }
                            case valtype::vt_f32: {
                                args.push_back(popOne<f32_t>(stack));
                                break;
                            }
                            case valtype::vt_f64: {
                                args.push_back(popOne<f64_t>(stack));
                                break;
                            }
                            default: {
                                scream("Unknown arg type met");
                            }
                        }
                    }

                    auto retVal = functionEval(
                            store,
                            moduleInst,
                            func.get_func(),
                            args
                    );

                    if (funcType.results_types.size() != 0) {
                        push(retVal);
                    }
                }

                case opcode::op_f32_const:
                case opcode::op_f64_const:
                case opcode::op_i64_const:
                case opcode::op_i32_const: {
                    pushArg(arg);
                    break;
                }

                case opcode::op_get_local: {
                    stack.push(getLocal(arg));
                    break;
                }
                case opcode::op_set_local: {
                    auto p1 = pop();
                    setLocal(arg, p1);
                }
                case opcode::op_tee_local: {
                    auto p1 = peek();
                    setLocal(arg, p1);
                }

                case opcode::op_i32_eq: {
                    auto [p1, p2] = popTwo<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == p2));
                    break;
                }
                case opcode::op_i64_eq: {
                    auto [p1, p2] = popTwo<i64_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == p2));
                    break;
                }
                case opcode::op_f32_eq: {
                    auto [p1, p2] = popTwo<f32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == p2));
                    break;
                }
                case opcode::op_f64_eq: {
                    auto [p1, p2] = popTwo<f64_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == p2));
                    break;
                }

                case opcode::op_i32_ne: {
                    auto [p1, p2] = popTwo<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 != p2));
                    break;
                }
                case opcode::op_i64_ne: {
                    auto [p1, p2] = popTwo<i64_t>(stack);
                    stack.push(static_cast<i32_t>(p1 != p2));
                    break;
                }
                case opcode::op_f32_ne: {
                    auto [p1, p2] = popTwo<f32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 != p2));
                    break;
                }
                case opcode::op_f64_ne: {
                    auto [p1, p2] = popTwo<f64_t>(stack);
                    stack.push(static_cast<i32_t>(p1 != p2));
                    break;
                }

                case opcode::op_i32_eqz: {
                    auto p1 = popOne<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == 0));
                    break;
                }
                case opcode::op_i64_eqz: {
                    auto p1 = popOne<i64_t>(stack);
                    stack.push(static_cast<i32_t>(p1 == 0));
                    break;
                }

                case opcode::op_i32_add: {
                    auto [p1, p2] = popTwo<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 + p2));
                    break;
                }
                case opcode::op_i64_add: {
                    auto [p1, p2] = popTwo<i64_t>(stack);
                    stack.push(static_cast<i64_t>(p1 + p2));
                    break;
                }
                case opcode::op_f32_add: {
                    auto [p1, p2] = popTwo<f32_t>(stack);
                    stack.push(static_cast<f32_t>(p1 + p2));
                    break;
                }
                case opcode::op_f64_add: {
                    auto [p1, p2] = popTwo<f64_t>(stack);
                    stack.push(static_cast<f64_t>(p1 + p2));
                    break;
                }

                case opcode::op_i32_sub: {
                    auto [p1, p2] = popTwo<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 - p2));
                    break;
                }
                case opcode::op_i64_sub: {
                    auto [p1, p2] = popTwo<i64_t>(stack);
                    stack.push(static_cast<i64_t>(p1 - p2));
                    break;
                }
                case opcode::op_f32_sub: {
                    auto [p1, p2] = popTwo<f32_t>(stack);
                    stack.push(static_cast<f32_t>(p1 - p2));
                    break;
                }
                case opcode::op_f64_sub: {
                    auto [p1, p2] = popTwo<f64_t>(stack);
                    stack.push(static_cast<f64_t>(p1 - p2));
                    break;
                }

                case opcode::op_i32_mul: {
                    auto [p1, p2] = popTwo<i32_t>(stack);
                    stack.push(static_cast<i32_t>(p1 * p2));
                    break;
                }
                case opcode::op_i64_mul: {
                    auto [p1, p2] = popTwo<i64_t>(stack);
                    stack.push(static_cast<i64_t>(p1 * p2));
                    break;
                }
                case opcode::op_f32_mul: {
                    auto [p1, p2] = popTwo<f32_t>(stack);
                    stack.push(static_cast<f32_t>(p1 * p2));
                    break;
                }
                case opcode::op_f64_mul: {
                    auto [p1, p2] = popTwo<f64_t>(stack);
                    stack.push(static_cast<f64_t>(p1 * p2));
                    break;
                }
            }
        }

        return stack.top();
    }
}
