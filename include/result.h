#pragma once
#include "query.h"
#include "serialize/sql.h"
#include <variant>

struct Result: public SqlSerializable {
    std::variant<Queries> command;

    template <typename T>
    Result(T&& command) 
        : command(std::forward<T>(command))
    {}

    void to_sql(std::ostream &os) const override;
};
