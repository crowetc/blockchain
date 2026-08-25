#include "Chain.hpp"

#include <algorithm>

namespace bc
{

Chain::
Chain(const std::vector<Transaction>& txs,
      std::uint32_t difficulty)
: difficulty_(difficulty)
{
    if (difficulty == 0 || difficulty > 64)
        throw std::invalid_argument("Invalid difficulty.");

    // Genesis block has no previous hash
    Block genesis(txs, "0", difficulty_);
    genesis.mine();
    this->add_block(genesis);
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

void
Chain::
add_block(const Block& blk)
{
    chain_.push_back(blk);
    block_hashes_.insert(blk.hash());
}

const Block&
Chain::
get_block(std::size_t index) const
{
    if (index >= chain_.size())
    {
        throw std::invalid_argument("Index does not exist.");  
    }

    return chain_[index];
}

bool
Chain::
contains(const std::string& hash) const
{
    return block_hashes_.find(hash) != block_hashes_.end();
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

} // namespace bc
