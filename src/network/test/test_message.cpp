#include <gtest/gtest.h>

#include "Block.hpp"
#include "Message.hpp"
#include "Transaction.hpp"

#include <string> 

// Basic encode and decode the same payload
TEST(message_test, encode_decode_round_trip)
{
    bc::Message m(bc::Message_type::NEW_TRANSACTION, "payload123");
    auto encoded = m.encode();

    auto decoded = bc::Message::decode(encoded);

    EXPECT_EQ(decoded.type, bc::Message_type::NEW_TRANSACTION);
    EXPECT_EQ(decoded.payload, "payload123");
}

// Check that Message_type is preserved through encode/decode
TEST(message_test, preserves_message_type)
{
    bc::Message m1(bc::Message_type::NEW_TRANSACTION, "tx");
    bc::Message m2(bc::Message_type::NEW_BLOCK, "blk");

    auto d1 = bc::Message::decode(m1.encode());
    auto d2 = bc::Message::decode(m2.encode());

    EXPECT_EQ(d1.type, bc::Message_type::NEW_TRANSACTION);
    EXPECT_EQ(d2.type, bc::Message_type::NEW_BLOCK);
}

// Check payloads with special characters
TEST(message_test, payload_with_delimiters)
{
    std::string payload = "hello|world|test";
    bc::Message m(bc::Message_type::NEW_TRANSACTION, payload);

    auto decoded = bc::Message::decode(m.encode());
    EXPECT_EQ(decoded.payload, payload);
}

// Check empty payload
TEST(message_test, empty_payload)
{
    bc::Message m(bc::Message_type::NEW_BLOCK, "");
    auto decoded = bc::Message::decode(m.encode());

    EXPECT_EQ(decoded.payload, "");
}

// Check decoding of malformed message with no delimiter
TEST(message_test, malformed_no_delimiter)
{
    std::string raw = "12345"; // no '|'

    auto decoded = bc::Message::decode(raw);

    EXPECT_EQ(decoded.type, bc::Message_type::NEW_TRANSACTION);
    EXPECT_TRUE(decoded.payload.empty());
}

// Check decoding of malformed message with an invalid Message_type
TEST(message_test, malformed_type_field)
{
    std::string raw = "not_an_int|payload";

    auto decoded = bc::Message::decode(raw);

    EXPECT_EQ(decoded.type, bc::Message_type::NEW_TRANSACTION);
    EXPECT_TRUE(decoded.payload.empty());
}

// Validate that the encoded message formats delimiters appropriately
TEST(message_test, encode_format)
{
    bc::Message m(bc::Message_type::NEW_TRANSACTION, "abc");
    auto encoded = m.encode();

    // Check that delimiter is in the correct position
    auto pos = encoded.find('|');
    EXPECT_NE(pos, std::string::npos);

    // Check that there is only one delimiter
    EXPECT_EQ(encoded.find('|', pos + 1), std::string::npos);
}

// Serialize and encode / decode and deserialize the same Transaction
TEST(message_test, carries_serialized_transaction)
{
    bc::Transaction tx("alice", "bob", 10);
    auto serialized = tx.serialize();

    bc::Message m(bc::Message_type::NEW_TRANSACTION, serialized);
    auto d = bc::Message::decode(m.encode());

    EXPECT_EQ(d.type, bc::Message_type::NEW_TRANSACTION);
    EXPECT_EQ(d.payload, serialized);

    auto tx2 = bc::Transaction::deserialize(d.payload);
    EXPECT_EQ(tx2.sender(), tx.sender());
    EXPECT_EQ(tx2.receiver(), tx.receiver());
    EXPECT_EQ(tx2.amount(), tx.amount());
}

// Serialize and encode / decode and deserialize the same Block
TEST(message_test, carries_serialized_block)
{
    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    txs.push_back(t1);

    bc::Block b(txs, "0000", 4);
    auto serialized = b.serialize();

    bc::Message m(bc::Message_type::NEW_BLOCK, serialized);
    auto d = bc::Message::decode(m.encode());

    EXPECT_EQ(d.type, bc::Message_type::NEW_BLOCK);
    EXPECT_EQ(d.payload, serialized);

    auto b2 = bc::Block::deserialize(d.payload);
    EXPECT_EQ(b2.hash(), b.hash());
}
