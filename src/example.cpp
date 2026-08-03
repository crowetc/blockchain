#include "Block.hpp"
#include "Chain.hpp"
#include "Transaction.hpp"

#include <iostream>
#include <memory>

using namespace bc;

int main()
{
    //
    // Create chain
    //
    uint32_t difficulty = 6;

    std::vector<Transaction> genesis_txs =
    {
        Transaction{"genesis_from", "genesis_to", 100}
    };

    std::cout << "Initializing chain..." << '\n';
    Chain chain = Chain(genesis_txs, difficulty);

    //
    // Block 1
    //
    std::cout << "Mining block 1..." << '\n';
    std::vector<Transaction> block1_txs =
    {
        Transaction{"alice", "bob", 50},
        Transaction{"carol", "dave", 20}
    };
    auto blk1 = chain.create_block(block1_txs);
    chain.add_block(blk1);

    //
    // Block 2
    //
    std::cout << "Mining block 2..." << '\n';
    std::vector<Transaction> block2_txs =
    {
        Transaction{"bob", "eve", 10}
    };
    auto blk2 = chain.create_block(block2_txs);
    chain.add_block(blk2);

    //
    // Block 3
    //
    std::cout << "Mining block 3..." << '\n';
    std::vector<Transaction> block3_txs =
    {
        Transaction{"dave", "frank", 5}
    };
    auto blk3 = chain.create_block(block3_txs);
    chain.add_block(blk3);

    //
    // Validate Chain
    //
    if (chain.validate())
    {
        std::cout << "Chain is valid" << '\n';
    }
    else
    {
        std::cout << "Chain failed... " << '\n';
    }

    return 0;
}
