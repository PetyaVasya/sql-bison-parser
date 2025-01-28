#pragma once
#include "serialize/sql.h"
#include <variant>

struct Type: SqlSerializable {

};

struct Integer : Type {
    int value;
    Integer(int value) 
        : value(value)
        {}

    void to_sql(std::ostream &os) const override { os << value; }
};

struct Text : Type {
    std::string value;

    Text(std::string value) 
        : value(std::move(value))
        {}

    Text(const char* value) 
        : value(value)
        {}

    void to_sql(std::ostream &os) const override {
        os << '\'';
        for (std::size_t i = 0; i < value.size(); ++i) {
            os << value.at(i);
            if (value.at(i) == '\'') {
                os << '\'';
            }
        }
        os << '\'';
        }
};

struct Literal : public std::variant<Integer, Text>, SqlSerializable {
    using std::variant<Integer, Text>::variant;

    void to_sql(std::ostream &os) const override {
        static const overloads converter {
            [&os](const auto& arg) { arg.to_sql(os); }
        };
        std::visit(converter, *this);
    }
};
