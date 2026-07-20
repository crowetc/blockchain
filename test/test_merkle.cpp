#include <gtest/gtest.h>
#include "Merkle_tree.hpp"
#include "Transaction.hpp"
#include "sha256.h"

#include <string>
#include <vector>

// Basic case: single transaction
TEST(merkle_test, single_transaction)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    
    bc::Merkle_tree tree(txs);

    const auto& levels = tree.levels();
    ASSERT_FALSE(levels.empty());
    EXPECT_EQ(levels.back().size(), 1u);      // root level has one hash
    EXPECT_EQ(tree.root(), levels.back()[0]); // root matches top level
}

// Two transactions
TEST(merkle_test, two_transactions)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    txs.emplace_back(bc::Transaction{"carol","dave",20,std::time(nullptr)});
    
    bc::Merkle_tree tree(txs);

    const auto& levels = tree.levels();
    ASSERT_FALSE(levels.empty());
    EXPECT_EQ(levels.front().size(), txs.size()); // leaf level size matches tx count
    EXPECT_EQ(levels.back().size(), 1u);          // root level has one hash
    EXPECT_EQ(tree.root(), levels.back()[0]);     // root matches top level
}

// Three transactions
TEST(merkle_test, three_transactions)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    txs.emplace_back(bc::Transaction{"carol","dave",20,std::time(nullptr)});
    txs.emplace_back(bc::Transaction{"eve","frank",30,std::time(nullptr)});

    bc::Merkle_tree tree{txs};

    const auto& levels = tree.levels();
    ASSERT_FALSE(levels.empty());
    EXPECT_EQ(levels.front().size(), txs.size());
    EXPECT_EQ(levels.back().size(), 1u);
    EXPECT_EQ(tree.root(), levels.back()[0]);
}

// Determinism: same transactions should produce same root
TEST(merkle_test, deterministic_root)
{
    std::vector<bc::Transaction> txs;
    txs.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    txs.emplace_back(bc::Transaction{"carol","dave",20,std::time(nullptr)});
    txs.emplace_back(bc::Transaction{"eve","frank",30,std::time(nullptr)});

    bc::Merkle_tree t1{txs};
    bc::Merkle_tree t2{txs};

    EXPECT_EQ(t1.root(), t2.root());
}

// Changing any transaction should change the root
TEST(merkle_test, root_changes_with_transaction)
{
    std::vector<bc::Transaction> txs1;
    txs1.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    txs1.emplace_back(bc::Transaction{"carol","dave",20,std::time(nullptr)});
    txs1.emplace_back(bc::Transaction{"eve","frank",30,std::time(nullptr)});

    std::vector<bc::Transaction> txs2;
    txs2.emplace_back(bc::Transaction{"alice","bob",10,std::time(nullptr)});
    txs2.emplace_back(bc::Transaction{"carol","dave",20,std::time(nullptr)});
    txs2.emplace_back(bc::Transaction{"eve","frank",31,std::time(nullptr)}); // different amount

    bc::Merkle_tree t1{txs1};
    bc::Merkle_tree t2{txs2};

    EXPECT_NE(t1.root(), t2.root());
}
