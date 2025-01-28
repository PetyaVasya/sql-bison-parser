#pragma once
#include "db/serialize/sql.h"
#include <string>
#include <optional>

struct Name: SqlSerializable {
    std::string name;

    Name() = default;

    Name(std::string name)
        : name(std::move(name))
        {}
    
    Name(const char * name)
        : name(name)
        {}

    void to_sql(std::ostream &os) const override;
};

struct AlternativeName : SqlSerializable {
    std::optional<Name> alternative;

    AlternativeName() = default;
    AlternativeName(Name value)
        : alternative(value)
        {}
    AlternativeName(std::optional<Name>&& alternative)
        : alternative(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};