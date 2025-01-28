#include "predicate.h"

void Predicate::to_sql(std::ostream &os) const {
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

void ValuePredicate::to_sql(std::ostream &os) const  {
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

void PredicateCompose::to_sql(std::ostream &os) const
{
    left.to_sql(os);
    Predicate::to_sql(os);
    right.to_sql(os);
}
