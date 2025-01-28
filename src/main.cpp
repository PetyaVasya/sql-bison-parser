#include <iostream>
#include "db/result.h"
#include "driver.h"

int main(int /*argc*/, char ** /*argv[]*/) {

    // std::vector<QualifiedValue> test;
    // Literal a{"test"};

    // test.push_back(
    //     std::move(a)
    // );
    DBDriver driver;

    std::cout << "bb";
    driver.parse("./tests/etc/simple.sql");
    std::cout << "keke";

    return 0;
}