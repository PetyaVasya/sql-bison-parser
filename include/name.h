#pragma once
#include <string>
#include <optional>

struct Name: SqlSerializable {
    std::string name;

    Name(std::string name)
        : name(std::move(name))
        {}
    
    Name(const char * name)
        : name(name)
        {}

    void to_sql(std::ostream &os) const override { os << name; }
};

struct AlternativeName : SqlSerializable {
    std::optional<Name> alternative;

    AlternativeName() = default;
    AlternativeName(Name value)
        : alternative(value)
        {}

    void to_sql(std::ostream &os) const override {
        if (alternative.has_value()) {
            os << " AS ";
            alternative.value().to_sql(os);
        }
    }
};