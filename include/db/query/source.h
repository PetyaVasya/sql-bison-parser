#pragma once

#include "db/serialize/sql.h"
#include "db/function.h"
#include "db/table.h"

struct Source : SqlSerializable {
    std::variant<FunctionCall, Table, std::monostate> source;

    template <typename T, std::enable_if_t<!std::is_same_v<Source, std::remove_reference_t<T>>, bool> = true>
    Source(T&& source)
        : source(std::forward<T>(source))
        {}
    
    void to_sql(std::ostream &os) const override;
};

struct QualifiedSource : Source, AlternativeName {
    using Source::Source;

    template <typename T, std::enable_if_t<!std::is_same_v<QualifiedSource, std::remove_reference_t<T>>, bool> = true>
    QualifiedSource(T&& source, Name alternative)
        : Source(std::forward<T>(source))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};