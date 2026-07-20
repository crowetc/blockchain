#include <gtest/gtest.h>
#include "Transaction.hpp"

// Basic validity
TEST(transaction_test, valid_transaction)
{
    bc::Transaction tx{"alice", "bob", 10, std::time(nullptr)};
    EXPECT_TRUE(tx.validate());
}

// Reject empty sender
TEST(transaction_test, rejects_empty_sender)
{
    bc::Transaction tx{"", "bob", 10, std::time(nullptr)};
    EXPECT_FALSE(tx.validate());
}

// Reject empty receiver
TEST(transaction_test, rejects_empty_receiver)
{
    bc::Transaction tx{"alice", "", 10, std::time(nullptr)};
    EXPECT_FALSE(tx.validate());
}

// Reject non-positive amounts
TEST(transaction_test, rejects_zero_amount)
{
    bc::Transaction tx{"alice", "bob", 0, std::time(nullptr)};
    EXPECT_FALSE(tx.validate());
}