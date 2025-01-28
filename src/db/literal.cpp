#include "db/literal.h"

void Integer::to_sql(std::ostream &os) const {
    os << value;
}

void Text::to_sql(std::ostream &os) const {
    os << '\'';
    for (std::size_t i = 0; i < value.size(); ++i) {
        os << value.at(i);
        if (value.at(i) == '\'') {
            os << '\'';
        }
    }
    os << '\'';
}

void Literal::to_sql(std::ostream &os) const {
    static const overloads converter {
        [&os](const auto& arg) { arg.to_sql(os); }
    };
    std::visit(converter, literal);
}
