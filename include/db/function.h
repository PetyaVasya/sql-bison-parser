#pragma once

struct FunctionCall;
struct ArrayCall;

#include "db/name.h"
#include "db/value.h"
#include <vector>

struct FunctionCall : Name {
    using Name::Name;
    std::vector<Value> args;

    FunctionCall() = default;

    FunctionCall(Name name, std::vector<Value> args)
        : Name(std::move(name))
        , args(std::move(args))
        {}

    FunctionCall(Name name, std::initializer_list<Value> args)
        : Name(std::move(name))
        , args(args)
        {}
    
    template <typename... T>
    FunctionCall(Name name, T&&... args)
        : FunctionCall(std::move(name), {std::forward<T>(args)...})
        {}

    void to_sql(std::ostream &os) const override {
        Name::to_sql(os);
        os << "(";
        SqlSerializable::to_sql(os, args);
        os << ")";
    }
};

struct ArrayCall: FunctionCall {

    ArrayCall()
        : FunctionCall("ARRAY")
        {}

    ArrayCall(std::vector<Value> args)
        : FunctionCall("ARRAY", std::move(args))
        {}

    ArrayCall(std::initializer_list<Value> args)
        : FunctionCall("ARRAY", args)
        {}

    template <typename... T>
    ArrayCall(T&&... args)
        : FunctionCall("ARRAY", std::forward<T>(args)...)
        {}

    void to_sql(std::ostream &os) const override {
        Name::to_sql(os);
        os << "[";
        SqlSerializable::to_sql(os, args);
        os << "]";
    }
};
