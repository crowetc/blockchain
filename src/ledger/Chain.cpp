#include "Chain.hpp"

#include <algorithm>

namespace bc
{

Chain::
Chain(const std::vector<Transaction>& txs,
      std::uint32_t difficulty)
: difficulty_(difficulty)
{
    // Genesis block has no previous hash
    Block genesis(txs, "0", difficulty_);
    genesis.mine();
    this->add_block(genesis);
}

bool
Chain::
validate() const
{
    for (size_t i = 1; i < chain_.size(); ++i)
    {
        const auto& prev = chain_[i-1];
        const auto& curr = chain_[i];

        // Check linkage
        if (curr.header.prev_hash != prev.hash()) { return false; }

        // Check block
        if (!curr.validate()) { return false; }
    }
    return true;
}

Block
Chain::
create_block(const std::vector<Transaction>& txs)
{
    const std::string prev_hash = chain_.back().hash();

    Block blk(txs, prev_hash, difficulty_);
    blk.mine();
    return blk;
}


} // namespace bc
