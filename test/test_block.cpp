#include <gtest/gtest.h>
#include "Block.hpp"
#include "Merkle_tree.hpp"
#include "Transaction.hpp"

#include <string>
#include <vector>

// Block construction
TEST(block_test, block_construction)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));
    txs.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));
    
    bc::Block b(txs, "0000", 0);

    EXPECT_FALSE(b.hash().empty());
    EXPECT_FALSE(b.header.merkle_root.empty());
}

// Merkle root should match Merkle_tree
TEST(block_test, root_matches_tree)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));
    txs.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));
    txs.emplace_back(bc::Transaction("eve","frank",30,std::time(nullptr)));

    bc::Block b(txs, "0000", 3);
    bc::Merkle_tree tree(txs);

    EXPECT_EQ(b.header.merkle_root, tree.root());
}

// Hash changes with transactions
TEST(block_test, hash_changes_with_transactions)
{
    std::vector<bc::Transaction> txs1;
    txs1.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));
    txs1.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));

    std::vector<bc::Transaction> txs2;
    txs2.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));
    txs2.emplace_back(bc::Transaction("carol","dave",21,std::time(nullptr))); // different amount

    bc::Block b1(txs1, "0000", 0);
    bc::Block b2(txs2, "0000", 0);

    EXPECT_NE(b1.hash(), b2.hash());
}

// Mining produces hash that matches required difficulty
TEST(block_test, mining_produces_valid_hash)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Block b(txs, "0000", 4);
    b.mine();

    const auto h = b.hash();
    EXPECT_EQ(h.substr(0, b.header.difficulty),
              std::string(b.header.difficulty, '0'));
}

// Validate should return true for a properly mined block
TEST(block_test, mining_produces_valid_block)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Block b(txs, "0000", 3);
    b.mine();

    EXPECT_TRUE(b.validate());
}

// Changing the block after mining should break validation
TEST(block_test, tampered_block_invalid)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Block b(txs, "0000", 3);
    b.mine();

    // Tamper with block body
    b.body.transactions.emplace_back(bc::Transaction("mallory","oscar",99,std::time(nullptr)));

    EXPECT_FALSE(b.validate());
}
