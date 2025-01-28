#pragma once
#include "db/literal.h"
#include "db/name.h"
#include "db/serialize/sql.h"

struct Value;
struct QualifiedValue;
struct SelectQuery;

#include "function.h"

struct Value : SqlSerializable {
    std::variant<Name, Literal, FunctionCall, ArrayCall, SelectQuery*> value;
    template <typename T>
    Value(T value)
        : value(std::move(value))
        {}

    void to_sql(std::ostream & os) const override;
};

struct QualifiedValue : Value, AlternativeName {
    using Value::Value;

    template <typename T>
    QualifiedValue(T value, Name alternative)
        : Value(std::move(value))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};
