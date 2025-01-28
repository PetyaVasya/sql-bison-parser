#pragma once
#include "db/literal.h"
#include "db/name.h"
#include "db/serialize/sql.h"

struct Value;
struct QualifiedValue;
struct SelectQuery;

#include "function.h"

struct Value : SqlSerializable {
    std::variant<Name, Literal, FunctionCall, ArrayCall, SelectQuery*, std::monostate> value;

    Value()
        : value(std::monostate())
        {}

    template <typename T, std::enable_if_t<!std::is_same_v<Value, std::remove_reference_t<T>>, bool> = true>
    Value(T&& value)
        : value(std::forward<T>(value))
        {}

    void to_sql(std::ostream & os) const override;
};

struct QualifiedValue : Value, AlternativeName {
    using Value::Value;

    QualifiedValue() = default;

    QualifiedValue(Value value, AlternativeName alternative)
        : Value(std::move(value))
        , AlternativeName(std::move(alternative))
        {}

    template <typename T, std::enable_if_t<!std::is_same_v<QualifiedValue, std::remove_reference_t<T>>, bool> = true>
    QualifiedValue(T&& value, Name alternative)
        : Value(std::forward<T>(value))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};
