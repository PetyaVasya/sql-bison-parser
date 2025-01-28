#include "value.h"
#include "query.h"

void Value::to_sql(std::ostream &os) const {
    static const overloads converter {
        [&os](const auto& arg) { arg.to_sql(os); },
        [&os](SelectQuery* arg) { arg->to_sql(os); },
        [&os](const SelectQuery* arg) { arg->to_sql(os); }
    };
    std::visit(converter, value);
}

void QualifiedValue::to_sql(std::ostream &os) const  {
    Value::to_sql(os);
    AlternativeName::to_sql(os);
}