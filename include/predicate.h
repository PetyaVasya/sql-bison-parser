#pragma once
#include "value.h"
#include "serialize/sql.h"
#include <variant>

struct Predicate : SqlSerializable {
    enum Type { AND, OR, VALUE };

    Type type;

    Predicate(Type type)
        : type(type)
    {}

    void to_sql(std::ostream &os) const override {
        switch (type) {
            case Type::AND:
                os << " AND ";
                break;
            case Type::OR:
                os << " OR ";
                break;
            default:
                break;
        }
    }
};

template <typename Left, typename Right>
struct TypedPredicate : Predicate {
    Left left;
    Right right;

    TypedPredicate(Predicate::Type type, Left left, Right right) 
        : Predicate(type)
        , left(std::move(left))
        , right(std::move(right))
        {}

    void to_sql(std::ostream &os) const override {
        left.to_sql(os);
        Predicate::to_sql(os);
        right.to_sql(os);
    }
};

struct ValuePredicate : TypedPredicate<Value, Value> {

    enum Operator { EQ, NEQ, LE, GR };

    Operator comp_operator;

    ValuePredicate(Operator comp_operator, Value left, Value right) 
        : TypedPredicate(Predicate::Type::VALUE, std::move(left), std::move(right))
        , comp_operator(comp_operator)
        {}
    
    void to_sql(std::ostream &os) const override {
        left.to_sql(os);
        switch (comp_operator) {
            case Operator::EQ:
                os << " = ";
                break;
            case Operator::NEQ:
                os << " != ";
                break;
            case Operator::LE:
                os << " < ";
                break;
            case Operator::GR:
                os << " > ";
                break;
        }
        right.to_sql(os);
    }
};

using PredicateCompose = TypedPredicate<Predicate, Predicate>;

struct OrPredicate : PredicateCompose {
    OrPredicate(Predicate left, Predicate right) 
        : TypedPredicate(Predicate::Type::OR, std::move(left), std::move(right))
        {}
};

struct AndPredicate : PredicateCompose {
    AndPredicate(Predicate left, Predicate right) 
        : TypedPredicate(Predicate::Type::AND, std::move(left), std::move(right))
        {}
};
