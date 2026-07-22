#include <gtest/gtest.h>
#include "Block.hpp"
#include "crypto_utils.hpp"
#include "Merkle_tree.hpp"
#include "Transaction.hpp"

#include <string>
#include <vector>

// Block construction
TEST(block_test, block_construction)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    bc::Transaction t2("carol","dave",20,std::time(nullptr));
    t1.sign(sk);
    t2.sign(sk);

    txs.push_back(t1);
    txs.push_back(t2);

    bc::Block b(txs, "0000", 0);

    EXPECT_FALSE(b.hash().empty());
    EXPECT_FALSE(b.header.merkle_root.empty());
}

// Merkle root should match Merkle_tree
TEST(block_test, root_matches_tree)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    bc::Transaction t2("carol","dave",20,std::time(nullptr));
    bc::Transaction t3("eve","frank",30,std::time(nullptr));

    t1.sign(sk);
    t2.sign(sk);
    t3.sign(sk);

    txs = {t1, t2, t3};

    bc::Block b(txs, "0000", 3);
    bc::Merkle_tree tree(txs);

    EXPECT_EQ(b.header.merkle_root, tree.root());
}

// Hash changes with transactions
TEST(block_test, hash_changes_with_transactions)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs1;
    bc::Transaction a1("alice","bob",10,std::time(nullptr));
    bc::Transaction a2("carol","dave",20,std::time(nullptr));
    a1.sign(sk);
    a2.sign(sk);

    txs1.push_back(a1);
    txs1.push_back(a2);

    std::vector<bc::Transaction> txs2;
    bc::Transaction b1("alice","bob",10,std::time(nullptr));
    bc::Transaction b2("carol","dave",21,std::time(nullptr)); // different amount
    b1.sign(sk);
    b2.sign(sk);

    txs2.push_back(b1);
    txs2.push_back(b2);
    
    bc::Block block1(txs1, "0000", 0);
    bc::Block block2(txs2, "0000", 0);

    EXPECT_NE(block1.hash(), block2.hash());
}

// Mining produces hash that matches required difficulty
TEST(block_test, mining_produces_valid_hash)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    t1.sign(sk);
    txs.push_back(t1);

    bc::Block b(txs, "0000", 4);
    b.mine();

    const auto h = b.hash();
    EXPECT_EQ(h.substr(0, b.header.difficulty),
              std::string(b.header.difficulty, '0'));
}

// Validate should return true for a properly mined block
TEST(block_test, mining_produces_valid_block)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    t1.sign(sk);
    txs.push_back(t1);

    bc::Block b(txs, "0000", 3);
    b.mine();

    EXPECT_TRUE(b.validate());
}

// Changing the block after mining should break validation
TEST(block_test, tampered_block_invalid)
{
    std::array<unsigned char, 32> pk{};
    std::array<unsigned char, 64> sk{};
    bc::generate_keypair(pk, sk);

    std::vector<bc::Transaction> txs;
    bc::Transaction t1("alice","bob",10,std::time(nullptr));
    t1.sign(sk);
    txs.push_back(t1);

    bc::Block b(txs, "0000", 3);
    b.mine();

    // Tamper with block body
    bc::Transaction bad("mallory","oscar",99,std::time(nullptr));
    bad.sign(sk);
    b.body.transactions.push_back(bad);

    EXPECT_FALSE(b.validate());
}
