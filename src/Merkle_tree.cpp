#include "crypto_utils.hpp"
#include "Merkle_tree.hpp"

namespace bc
{

Merkle_tree::
Merkle_tree(const std::vector<Transaction>& txs)
{
    build(txs);
}

void
Merkle_tree::
build(const std::vector<Transaction>& txs)
{
    levels_.clear();
    level_t current;

    // Hash each transaction into a leaf
    for (const auto& tx : txs)
    {
        current.push_back(sha256_hex(tx.serialize()));
    }

    levels_.push_back(current);

    // Build up tree
    while (current.size() > 1)
    {
        level_t next;

        // hash together pairs from previous level
        for (size_t i = 0; i < current.size(); i += 2)
        {
            if (i + 1 < current.size())
            {
                next.push_back(sha256_hex(current[i] + current[i + 1]));
            }
            else
            {
                // duplicate last node if odd count
                next.push_back(sha256_hex(current[i] + current[i]));
            }
        }

        levels_.push_back(next);
        current = next;
    }

    root_ = current[0];
}

} // namespace bc
