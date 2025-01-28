#pragma once
#include <ostream>

struct SqlSerializable {
    virtual void to_sql(std::ostream &os) const = 0;
    virtual ~SqlSerializable() = default;
protected:

    template <typename T>
    static void to_sql(std::ostream &os, const std::vector<T>& elements, std::string delimiter = ", ") {
        bool first = true;
        for (const auto& element : elements) {
            if (first) { 
                first = false;
            } else {
                os << delimiter;
            }
            element.to_sql(os);
        }
    }
};

template<class... Ts>
struct overloads : Ts... { using Ts::operator()...; };
template<class... Ts> overloads(Ts...) -> overloads<Ts...>;

