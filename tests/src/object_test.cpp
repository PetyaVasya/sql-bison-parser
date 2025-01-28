#include "db/result.h"
#include "driver.h"
#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <sstream>

TEST(ParserObjectTest, SimpleTest)
{
    DBDriver driver;

    EXPECT_EQ(driver.parse("tests/etc/simple.sql"), 0);
    EXPECT_TRUE(driver.result.has_value());
    
    std::ifstream sql{"tests/etc/simple.sql"};
    std::stringstream buffer;
    buffer << sql.rdbuf();
    sql.close();

    std::ostringstream result;
    driver.result.value().to_sql(result);

    EXPECT_EQ(buffer.str(), result.str());
}

TEST(ParserObjectTest, BaseTest)
{
    DBDriver driver;

    EXPECT_EQ(driver.parse("tests/etc/query.sql"), 0);
    EXPECT_TRUE(driver.result.has_value());
    
    std::ifstream sql{"tests/etc/query.sql"};
    std::stringstream buffer;
    buffer << sql.rdbuf();
    sql.close();

    std::ostringstream result;
    driver.result.value().to_sql(result);

    EXPECT_EQ(buffer.str(), result.str());
}
