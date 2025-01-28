#pragma once
#include "literal.h"
#include "name.h"
#include "serialize/sql.h"

struct Value;

#include "function.h"

struct Value : std::variant<Name, Literal, FunctionCall, ArrayCall>, SqlSerializable {
    using std::variant<Name, Literal, FunctionCall, ArrayCall>::variant;

    void to_sql(std::ostream &os) const override {
        static const overloads converter {
            [&os](const auto& arg) { arg.to_sql(os); }
        };
        std::visit(converter, *this);
    }
};

struct QualifiedValue : Value, AlternativeName {
    using Value::Value;

    template <typename T>
    QualifiedValue(T value, Name alternative)
        : Value(std::move(value))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override  {
        Value::to_sql(os);
        AlternativeName::to_sql(os);
    }
};