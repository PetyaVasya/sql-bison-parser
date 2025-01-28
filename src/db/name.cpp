#include "db/name.h"

void Name::to_sql(std::ostream &os) const {
    os << name;
}

void AlternativeName::to_sql(std::ostream &os) const {
    if (alternative.has_value()) {
        os << " AS ";
        alternative.value().to_sql(os);
    }
}