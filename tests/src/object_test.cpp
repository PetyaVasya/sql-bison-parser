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
    
    // driver.result
}

TEST(ParserObjectTest, BaseTest)
{
    DBDriver driver;

    EXPECT_EQ(driver.parse("tests/etc/query.sql"), 0);
}