#include <gtest/gtest.h>
#include "crypto_utils.hpp"
#include "Transaction.hpp"

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

// Signed transaction validates
TEST(transaction_test, signed_transaction_validates)
{
    // Generate keypair
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    bc::Transaction tx("alice", "bob", 10);
    tx.sign(sk);

    EXPECT_TRUE(tx.validate());
}
