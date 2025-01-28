#pragma once

#include "db/query/query.h"
#include "db/serialize/sql.h"
#include <variant>

struct Result: public SqlSerializable {
    std::variant<Queries> command;

    template <typename T, std::enable_if_t<!std::is_same_v<Result, std::remove_reference_t<T>>, bool> = true>
    Result(T&& command) 
        : command(std::forward<T>(command))
    {}

    void to_sql(std::ostream &os) const override;
};
