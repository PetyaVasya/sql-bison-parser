#include "db/query/query.h"

void Source::to_sql(std::ostream &os) const {
    static const overloads converter {
        [&os](const auto& arg) { arg.to_sql(os); },
        [](const std::monostate&) {}
    };
    std::visit(converter, source);
}

void QualifiedSource::to_sql(std::ostream &os) const {
    Source::to_sql(os);
    AlternativeName::to_sql(os);
}

void FromClause::to_sql(std::ostream &os) const {
    os << "FROM ";
    SqlSerializable::to_sql(os, sources);
}

void WhereClause::to_sql(std::ostream &os) const {
    os << "WHERE ";
    predicate->to_sql(os);
}

void JoinClause::to_sql(std::ostream &os) const {
    os << "JOIN ";
    source.to_sql(os);
    if (predicate.get()) {
        os << " ON ";
        predicate->to_sql(os);
    }
}

void BaseQuery::to_sql(std::ostream &os) const {
    if (from.has_value()) {
        from.value().to_sql(os);
        os << " ";
    }
    if (where.has_value()) {
        where.value().to_sql(os);
        os << " ";
    }
}

void SelectQuery::to_sql(std::ostream &os) const {
    os << "SELECT ";
    if (!values.empty()) {
        SqlSerializable::to_sql(os, values);
        os << " ";
    }
    if (from.has_value()) {
        from.value().to_sql(os);
        os << " ";
    }
    if (!joins.empty()) {
        SqlSerializable::to_sql(os, joins, " ");
        os << " ";
    }
    if (where.has_value()) {
        where.value().to_sql(os);
        os << " ";
    }
}

void Queries::to_sql(std::ostream &os) const {
    static const overloads converter {
        [&os](const auto& arg) { arg.to_sql(os); },
        [](const std::monostate) {}
    };

    std::visit(
        converter,
        query
    );
}