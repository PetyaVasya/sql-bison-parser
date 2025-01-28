#pragma once

#include "db/serialize/sql.h"
#include "db/predicate.h"
#include "db/query/source.h"
#include <vector>
#include <optional>

struct FromClause : SqlSerializable {
    std::vector<QualifiedSource> sources;
    FromClause() = default;
    FromClause(std::initializer_list<QualifiedSource> args)
        : sources(args)
        {}

    template <typename T, std::enable_if_t<!std::is_same_v<FromClause, std::remove_reference_t<T>>, bool> = true>
    FromClause(T&& args)
        : FromClause({std::forward<T>(args),})
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
        std::vector<QualifiedValue> values,
        std::optional<FromClause> from = {},
        std::optional<WhereClause> where = {},
        std::vector<JoinClause> joins = {}
    )
    : BaseQuery{std::move(from), std::move(where)}
    , values(values)
    , joins(std::move(joins))
    {}

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

struct Queries : SqlSerializable {
    std::variant<SelectQuery, std::monostate> query;

    Queries()
        : query(std::monostate())
        {}

    template <typename T, std::enable_if_t<!std::is_same_v<Queries, std::remove_reference_t<T>>, bool> = true>
    Queries(T&& query)
        : query(std::forward<T>(query))
        {}

    void to_sql(std::ostream& os) const override;
};
