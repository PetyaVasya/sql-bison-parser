#include "db/result.h"

void Result::to_sql(std::ostream &os) const {
    static const overloads converter {
        [&os](const auto& arg) { arg.to_sql(os); }
    };
    std::visit(
        converter,
        command
    );
}