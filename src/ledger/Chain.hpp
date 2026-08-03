#ifndef BLOCKCHAIN_HPP_
#define BLOCKCHAIN_HPP_

#include <cstdint>
#include <memory>
#include <unordered_set>
#include <vector>

#include "Block.hpp"
#include "Transaction.hpp"

namespace bc
{

/** Simple Blockchain
 * 
 *  The chain class manages
 *  - creation of new blocks
 *  - proof-of-work mining
 *  - validation of blocks
 */
class Chain
{
public:
    /** Construct a new blockchain.
     *  @param txs the transactions to include in the genesis block
     *  @param difficulty the mining difficulty for each block (the number of leading zeros required for a valid hash)
     *
     *  The chain is initialized with a single genesis block.
     */
    Chain(const std::vector<Transaction>& txs,
          uint32_t difficulty);

    /** The number of blocks in the chain. */
    std::size_t
    size() const;

    /** The mining difficulty. */
    uint32_t
    difficulty() const;
    
    /** Create and mine a new block.
     *  @param txs the transactions to include in the block.
     *
     *  Constructs a block using the current chain tip and performs
     *  proof‑of‑work. The caller is responsible for adding the mined
     *  block to the chain.
     */
    Block
    create_block(const std::vector<Transaction>& txs);

    /** Append an existing block to the chain.
     *
     *  Used for incorporating blocks received from the network or
     *  resolved orphan blocks. Assumes the block has already been
     *  validated.
     */
    void
    add_block(const Block& blk);

    /** Return a block by index.
     *  @param index the index of the block to return.
     */
    const Block&
    get_block(std::size_t index) const;

    /** Validate the entire chain.
     *  @return @c true if the chain is valid
     *  @return @c false if any block is inconsistent
     *
     *  Validation checks:
     *  - each block's stored hash matches its computed hash
     *  - each blocks prev_hash matches the hash of the block before it
     */
    bool
    validate() const;

private:
    uint32_t difficulty_;
    std::vector<Block> chain_;
    std::unordered_set<std::string> block_hashes_;
};

//
// Inline Implementations
//

inline
std::size_t
Chain::
size() const
{
    return chain_.size();
}

inline
uint32_t
Chain::
difficulty() const
{
    return difficulty_;
}

inline
void
Chain::
add_block(const Block& blk)
{
    chain_.push_back(blk);
}

inline
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

} // namespace bc

#endif // BLOCKCHAIN_HPP_
