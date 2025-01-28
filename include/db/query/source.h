#pragma once

#include "db/serialize/sql.h"
#include "db/function.h"
#include "db/table.h"

struct Source : SqlSerializable {
    std::variant<FunctionCall, Table> source;

    template <typename T>
    Source(T&& source)
        : source(std::forward<T>(source))
        {}
    
    void to_sql(std::ostream &os) const override;
};

struct QualifiedSource : Source, AlternativeName {
    using Source::Source;

    template <typename T>
    QualifiedSource(T&& source, Name alternative)
        : Source(std::forward<T>(source))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};