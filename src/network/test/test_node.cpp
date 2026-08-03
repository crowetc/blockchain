#include <gtest/gtest.h>

#include "Node.hpp"
#include "Test_peer.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include "Message.hpp"

// Helper: simple transaction
static bc::Transaction
make_tx()
{
    return bc::Transaction("alice", "bob", 10);
}

// Helper: simple block
static bc::Block
make_block()
{
    return bc::Block({}, "prevhash", 1);
}

// Verify broadcast sends encoded message to all peers
TEST(node_test, broadcast_sends_to_all_peers)
{
    bc::Node node(0, 1);

    auto p1 = std::make_unique<bc::Test_peer>("A", 1000);
    auto p2 = std::make_unique<bc::Test_peer>("B", 2000);

    auto* p1_raw = p1.get();
    auto* p2_raw = p2.get();

    node.add_peer(std::move(p1));
    node.add_peer(std::move(p2));

    bc::Message msg(bc::Message_type::NEW_TRANSACTION, "hello");
    node.broadcast(msg);

    ASSERT_EQ(p1_raw->sent().size(), 1);
    ASSERT_EQ(p2_raw->sent().size(), 1);

    EXPECT_EQ(p1_raw->sent().back(), msg.encode());
    EXPECT_EQ(p2_raw->sent().back(), msg.encode());
}

// Verify submit broadcasts NEW_TRANSACTION
TEST(node_test, submit_broadcasts_new_transaction)
{
    bc::Node node(0, 1);

    auto p = std::make_unique<bc::Test_peer>("A", 1000);
    auto* raw = p.get();
    node.add_peer(std::move(p));

    auto tx = make_tx();
    node.submit(tx);

    ASSERT_EQ(raw->sent().size(), 1);

    auto msg = bc::Message::decode(raw->sent().back());
    EXPECT_EQ(msg.type, bc::Message_type::NEW_TRANSACTION);

    auto decoded = bc::Transaction::deserialize(msg.payload);
    EXPECT_EQ(decoded.sender(), tx.sender());
    EXPECT_EQ(decoded.receiver(), tx.receiver());
    EXPECT_EQ(decoded.amount(), tx.amount());
}

// Verify inbound NEW_TRANSACTION adds to mempool (indirectly tested via mine())
TEST(node_test, node_processes_new_transaction)
{
    bc::Node node(0, 1);

    // Create a peer to observe broadcasts from mine()
    auto p = std::make_unique<bc::Test_peer>("A", 1000);
    auto* raw = p.get();
    node.add_peer(std::move(p));

    // Simulate inbound NEW_TRANSACTION
    auto tx = make_tx();
    bc::Message msg(bc::Message_type::NEW_TRANSACTION, tx.serialize());

    // Feed the message through the inbox
    raw->push_inbox(msg.encode());

    // Run one listen iteration manually (no threads)
    {
        auto raw_msg = raw->receive();
        if (!raw_msg.empty())
        {
            node.receive_message(raw_msg);
        }
    }

    // Now mine — the tx should appear in the block
    node.mine();

    ASSERT_FALSE(raw->sent().empty());

    auto mined_msg = bc::Message::decode(raw->sent().back());
    EXPECT_EQ(mined_msg.type, bc::Message_type::NEW_BLOCK);

    auto blk = bc::Block::deserialize(mined_msg.payload);
    EXPECT_FALSE(blk.body.transactions.empty());
}

// Verify mine broadcasts NEW_BLOCK
TEST(node_test, mine_broadcasts_new_block)
{
    bc::Node node(0, 1);

    auto p = std::make_unique<bc::Test_peer>("A", 1000);
    auto* raw = p.get();
    node.add_peer(std::move(p));

    auto tx = make_tx();
    node.submit(tx);

    node.mine();

    ASSERT_FALSE(raw->sent().empty());

    auto msg = bc::Message::decode(raw->sent().back());
    EXPECT_EQ(msg.type, bc::Message_type::NEW_BLOCK);

    auto blk = bc::Block::deserialize(msg.payload);
    EXPECT_FALSE(blk.body.transactions.empty());
}