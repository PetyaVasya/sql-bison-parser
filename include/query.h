#pragma once

#include "serialize/sql.h"
#include "predicate.h"
#include "table.h"
#include <vector>
#include <optional>

struct Source : public std::variant<FunctionCall, Table>, SqlSerializable {
    using std::variant<FunctionCall, Table>::variant;
    
    void to_sql(std::ostream &os) const override;
};

struct QualifiedSource : Source, AlternativeName {
    using Source::Source;

    template <typename T>
    QualifiedSource(T source, Name alternative)
        : Source(std::move(source))
        , AlternativeName(std::move(alternative))
        {}

    void to_sql(std::ostream &os) const override;
};

struct FromClause : SqlSerializable {
    std::vector<QualifiedSource> sources;
    FromClause(std::initializer_list<QualifiedSource> args)
        : sources(args)
        {}

    template <typename... T>
    FromClause(T&&... args)
        : FromClause({std::forward<T>(args)...})
        {}

    void to_sql(std::ostream &os) const override;
};

struct WhereClause : SqlSerializable {
    std::unique_ptr<Predicate> predicate;

    WhereClause(std::unique_ptr<Predicate> predicate)
        : predicate(std::move(predicate))
        {}

    void to_sql(std::ostream &os) const override;
};

struct JoinClause : SqlSerializable {
    QualifiedSource source;
    std::unique_ptr<Predicate> predicate;

    JoinClause(
        QualifiedSource source,
        std::unique_ptr<Predicate> predicate
    )
    : source(std::move(source))
    , predicate(std::move(predicate))
    {}

    void to_sql(std::ostream &os) const override;
};

struct Query : SqlSerializable {
};

struct BaseQuery : Query {
    std::optional<FromClause> from;
    std::optional<WhereClause> where;

    BaseQuery(
        std::optional<FromClause> from = {},
        std::optional<WhereClause> where = {}
    )
    : from{std::move(from)}
    , where{std::move(where)}
    {}

    void to_sql(std::ostream &os) const override;
};

struct SelectQuery : BaseQuery {
    std::vector<QualifiedValue> values;
    std::vector<JoinClause> joins;

    SelectQuery() = default;

    SelectQuery(
        std::initializer_list<QualifiedValue> values,
        std::optional<FromClause> from = {},
        std::optional<WhereClause> where = {},
        std::vector<JoinClause> joins = {}
    )
    : BaseQuery{std::move(from), std::move(where)}
    , values(values)
    , joins(std::move(joins))
    {}

    void to_sql(std::ostream &os) const override;
};

struct Queries : public std::variant<SelectQuery>, SqlSerializable {
    using std::variant<SelectQuery>::variant;
    void to_sql(std::ostream& os) const override;
};
