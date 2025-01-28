#include "result.h"
#include <gtest/gtest.h>

#include <fstream>
#include <memory>
#include <sstream>

TEST(ParserTest, TestOutput)
{
    std::ifstream sql{"tests/etc/query.sql"};
    std::stringstream buffer;
    buffer << sql.rdbuf();

    std::vector<JoinClause> joins;
    joins.emplace_back(
        QualifiedSource{
            FunctionCall{
                "s3",
                Literal{"fit_big_customer/*.parquet"}
            },
            "parquet_data"
        },
        std::make_unique<ValuePredicate>(
            ValuePredicate::EQ,
            Name{"customer.customer_id"},
            Name{"parquet_data.customer_id"}
        )
    );
    joins.emplace_back(
        QualifiedSource{
            FunctionCall{
                "mongodb",
                Literal{"dbname.customer"},
                Literal{"mongo query"}
            },
            "mongo_data"
        },
        std::make_unique<ValuePredicate>(
            ValuePredicate::EQ,
            Name{"customer.customer_id"},
            Name{"mongo_data.customer_id"}
        )
    );
    Result query {
        Queries{
            SelectQuery{
                {
                    Name{"customer.customer_id"},
                    QualifiedValue{
                        FunctionCall{
                            "ml_predict",
                            ArrayCall{
                                Name{"customer.total_transactions"},
                                Name{"customer.total_amount_spent"},
                                Name{"customer.avg_transaction_value"}
                            }
                        },
                        "churn_probability"
                    },
                    Name{"mongo_data.customer_segment"}
                },
                FromClause {
                    QualifiedSource{
                        FunctionCall{
                            "postgresql",
                            Literal{"public.customers"},
                            Literal{"sql query"}
                        },
                        "customer"
                    }
                },
                {},
                std::move(joins)
            }
        }
    };

    std::ostringstream result;
    query.to_sql(result);

    
    EXPECT_EQ(buffer.str(), result.str());
}