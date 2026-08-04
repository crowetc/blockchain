#include "Block.hpp"
#include "Chain.hpp"
#include "Transaction.hpp"

#include <iostream>
#include <memory>

using namespace bc;

void print_block(const Block& blk, int index)
{
    std::cout << "\n=== Block " << index << " ===\n";
    std::cout << "Prev Hash:   " << blk.header.prev_hash << "\n";
    std::cout << "Merkle Root: " << blk.header.merkle_root << "\n";
    std::cout << "Timestamp:   " << blk.header.time << "\n";
    std::cout << "Difficulty:  " << blk.header.difficulty << "\n";
    std::cout << "Nonce:       " << blk.header.nonce << "\n";
    std::cout << "Hash:        " << blk.hash() << "\n";

    std::cout << "Transactions:\n";
    for (const auto& tx : blk.body.transactions)
    {
        std::cout << "  - " << tx.sender() << " -> " << tx.receiver()
                  << " (" << tx.amount() << ")\n";
    }

    std::cout << "====================\n\n";
}

int main()
{
    uint32_t difficulty = 4;

    std::vector<Transaction> genesis_txs =
    {
        {"genesis_from", "genesis_to", 100}
    };

    std::cout << "\033[34mInitializing chain...\033[0m\n";
    Chain chain(genesis_txs, difficulty);

    // Block 1
    std::cout << "\033[33mMining block 1...\033[0m\n";
    auto blk1 = chain.create_block({
        {"alice", "bob", 50},
        {"carol", "dave", 20}
    });
    chain.add_block(blk1);
    print_block(blk1, 1);

    // Block 2
    std::cout << "\033[33mMining block 2...\033[0m\n";
    auto blk2 = chain.create_block({
        {"bob", "eve", 10}
    });
    chain.add_block(blk2);
    print_block(blk2, 2);

    // Block 3
    std::cout << "\033[33mMining block 3...\033[0m\n";
    auto blk3 = chain.create_block({
        {"dave", "frank", 5}
    });
    chain.add_block(blk3);
    print_block(blk3, 3);

    // Validate
    std::cout << "\033[34mValidating chain...\033[0m\n";
    if (chain.validate())
        std::cout << "\033[32mChain is valid!\033[0m\n";
    else
        std::cout << "\033[31mChain failed.\033[0m\n";

    return 0;
}
