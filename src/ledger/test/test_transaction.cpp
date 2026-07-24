#include <gtest/gtest.h>
#include "crypto_utils.hpp"
#include "Transaction.hpp"

// Unsigned transaction
TEST(transaction_test, unsigned_transaction)
{
    bc::Transaction tx("alice", "bob", 10, std::time(nullptr));
    EXPECT_FALSE(tx.validate());
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

// Tampering invalidates signature
TEST(transaction_test, tampering_invalidates_signature)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    // Original transaction
    bc::Transaction tx("alice", "bob", 10);
    tx.sign(sk);
    EXPECT_TRUE(tx.validate());

    // Tampered message (same fields, but different amount)
    bc::Transaction tampered("alice", "bob", 999);

    // Validate original signature against tampered message
    EXPECT_FALSE(bc::verify(tampered.serialize(), tx.signature(), tx.public_key()));
}

// Wrong key fails
TEST(transaction_test, wrong_key_fails_validation)
{
    // Keypair A
    std::array<unsigned char, 32> pkA{};
    std::array<unsigned char, 64> skA{};
    bc::generate_keypair(pkA, skA);

    // Keypair B
    std::array<unsigned char, 32> pkB{};
    std::array<unsigned char, 64> skB{};
    bc::generate_keypair(pkB, skB);

    // Transaction signed with key A
    bc::Transaction txA("alice", "bob", 10);
    txA.sign(skA);
    EXPECT_TRUE(txA.validate());

    // A different transaction signed with key B
    bc::Transaction txB("alice", "bob", 10);
    txB.sign(skB);
    EXPECT_TRUE(txB.validate());

    // Check that signature A does NOT validate under key B
    EXPECT_FALSE(bc::verify(txA.serialize(), txA.signature(), pkB));
}