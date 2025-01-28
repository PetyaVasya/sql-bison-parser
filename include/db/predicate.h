#pragma once
#include "db/serialize/sql.h"
#include "value.h"
#include <variant>

struct Predicate : SqlSerializable {
    enum Type { AND, OR, VALUE };

    Type type;

    Predicate(Type type)
        : type(type)
    {}

    void to_sql(std::ostream &os) const override;
};

struct ValuePredicate : Predicate {
    enum Operator { EQ, NEQ, LE, GR };

    Value left;
    Value right;
    Operator comp_operator;

    ValuePredicate(Operator comp_operator, Value left, Value right) 
        : Predicate(Predicate::Type::VALUE)
        , left(std::move(left))
        , right(std::move(right))
        , comp_operator(comp_operator)
        {}
    
    void to_sql(std::ostream &os) const override;
};

struct PredicateCompose : Predicate {
    Predicate left;
    Predicate right;

    PredicateCompose(Predicate::Type type, Predicate left, Predicate right) 
        : Predicate(type)
        , left(std::move(left))
        , right(std::move(right))
        {}

    void to_sql(std::ostream &os) const override;
};

struct OrPredicate : PredicateCompose {
    OrPredicate(Predicate left, Predicate right) 
        : PredicateCompose(Predicate::Type::OR, std::move(left), std::move(right))
        {}
};

struct AndPredicate : PredicateCompose {
    AndPredicate(Predicate left, Predicate right) 
        : PredicateCompose(Predicate::Type::AND, std::move(left), std::move(right))
        {}
};
