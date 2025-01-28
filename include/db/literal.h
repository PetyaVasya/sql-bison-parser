#pragma once
#include "db/serialize/sql.h"
#include <variant>

struct Type: SqlSerializable {

};

struct Integer : Type {
    int value;
    Integer(int value) 
        : value(value)
        {}

    void to_sql(std::ostream &os) const override;
};

struct Text : Type {
    std::string value;

    Text(std::string value) 
        : value(std::move(value))
        {}

    Text(const char* value) 
        : value(value)
        {}

    void to_sql(std::ostream &os) const override;
};

struct Literal : SqlSerializable {
    std::variant<Integer, Text> literal;

    template <typename T>
    Literal(T&& litreal)
        : literal(std::forward<T>(litreal))
        {}

    void to_sql(std::ostream &os) const override;
};
