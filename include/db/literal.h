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
    std::variant<Integer, Text, std::monostate> literal;

    Literal()
        : literal(std::monostate())
        {}

    template <typename T, std::enable_if_t<!std::is_same_v<Literal, std::remove_reference_t<T>>, bool> = true>
    Literal(T&& literal)
        : literal(std::forward<T>(literal))
        {}

    void to_sql(std::ostream &os) const override;
};
