#include <gtest/gtest.h>
#include "Transaction.hpp"

// Basic validity
TEST(transaction_test, valid_transaction)
{
    bc::Transaction tx("alice", "bob", 10, std::time(nullptr));
    EXPECT_TRUE(tx.validate());
}

// Empty sender throws
TEST(transaction_test, empty_sender_throws)
{
    EXPECT_THROW(bc::Transaction tx("", "bob", 10, std::time(nullptr)), std::invalid_argument);
}

// Empty receiver throws
TEST(transaction_test, empty_receiver_throws)
{
    EXPECT_THROW(bc::Transaction tx("alice", "", 10, std::time(nullptr)), std::invalid_argument);
}

// Reject non-positive amounts
TEST(transaction_test, rejects_zero_amount)
{
    EXPECT_THROW(bc::Transaction tx("alice", "bob", 0, std::time(nullptr)), std::invalid_argument);
}