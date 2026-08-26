#include <gtest/gtest.h>
#include "crypto_utils.hpp"
#include "Transaction.hpp"

#include <array>
#include <ctime>
#include <stdexcept>

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

// Reject zero amounts
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

// Unsigned transaction does not validate
TEST(transaction_test, unsigned_transaction_fails_validation)
{
    bc::Transaction tx("alice", "bob", 10);

    EXPECT_FALSE(tx.validate());
}

// Serialized signed transaction can be restored and validated
TEST(transaction_test, signed_transaction_round_trip)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};

    bc::generate_keypair(pk, sk);

    bc::Transaction tx("alice", "bob", 10, std::time(nullptr));
    tx.sign(sk);

    const std::string serialized = tx.serialize();

    bc::Transaction restored = bc::Transaction::deserialize(serialized);

    EXPECT_EQ(restored.sender(), tx.sender());

    EXPECT_EQ(restored.receiver(), tx.receiver());

    EXPECT_EQ(restored.amount(), tx.amount());

    EXPECT_EQ(restored.timestamp(), tx.timestamp());

    EXPECT_EQ(restored.public_key(), tx.public_key());

    EXPECT_EQ(restored.signature(), tx.signature());

    EXPECT_TRUE(restored.validate());
}

// Malformed transaction throws
TEST(transaction_test, malformed_transaction_throws)
{
    EXPECT_THROW(
        bc::Transaction::deserialize("alice|bob|10"),
        std::invalid_argument
    );
}

// Invalid amount throws
TEST(transaction_test, invalid_amount_throws)
{
    EXPECT_THROW(
        bc::Transaction::deserialize(
            "alice|bob|not_a_number|123|abcd|abcd"
        ),
        std::invalid_argument
    );
}
