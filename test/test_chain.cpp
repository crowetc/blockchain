#include <gtest/gtest.h>
#include "Block.hpp"
#include "Chain.hpp"
#include "Transaction.hpp"

#include <string>
#include <vector>

// Genesis block is created on construction
TEST(chain_test, GenesisBlockCreated)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Chain chain(txs, 3);

    EXPECT_EQ(chain.size(), 1u);
    EXPECT_EQ(chain.difficulty(), 3u);

    const auto& genesis = chain.get_block(0);
    EXPECT_FALSE(genesis.hash().empty());
}

// Adding a block increases chain size
TEST(chain_test, add_block_increases_size)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Chain chain(txs, 3);
    EXPECT_EQ(chain.size(), 1u);

    std::vector<bc::Transaction> txs2;
    txs2.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));

    chain.add_block(txs2);
    EXPECT_EQ(chain.size(), 2u);
}

// Chain validation should succeed for a properly built chain
TEST(chain_test, valid_chain_passes_validation)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Chain chain(txs, 3);
    EXPECT_EQ(chain.size(), 1u);

    std::vector<bc::Transaction> txs2;
    txs2.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));

    chain.add_block(txs2);

    EXPECT_TRUE(chain.validate());
}

// Tampering with a block hash should break validation
TEST(chain_test, tampered_hash_fails_validation)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Chain chain(txs, 3);
    EXPECT_EQ(chain.size(), 1u);

    std::vector<bc::Transaction> txs2;
    txs2.emplace_back(bc::Transaction("carol","dave",20,std::time(nullptr)));

    chain.add_block(txs2);
    EXPECT_TRUE(chain.validate());

    // Tamper with second block's hash via const_cast
    const auto& b1_const = chain.get_block(1);
    auto& b1 = const_cast<bc::Block&>(b1_const);
    b1.header.prev_hash = "FAKEHASH";  // break linkage

    EXPECT_FALSE(chain.validate());
}

// Out-of-range get_block should throw
TEST(ChainTest, block_out_of_range_throws)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction("alice","bob",10,std::time(nullptr)));

    bc::Chain chain(txs, 3);

    EXPECT_THROW(chain.get_block(1), std::invalid_argument);
}
