#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <cstdint>
#include <ctime>
#include <iostream>
#include <vector>

#include "Transaction.hpp"

namespace bc
{

/** Block header
 * 
 *  Contains all consensus-critical fields that are hashed during mining
 */
struct Block_header
{
    std::string prev_hash;   // hash of the previous block
    std::string merkle_root; // merkle root of all transactions
    time_t time;             // timestamp
    uint64_t nonce;          // proof-of-work nonce
    uint32_t difficulty;     // mining difficulty
};

/** Block body
 * 
 *  Contains all transaction data stored in the block
 */
struct Block_body
{
    std::vector<Transaction> transactions;
};

/** Block
 *
 *  Consists of a public header and body
 */
class Block
{
public:
    Block_header header; // Public header (protocol data)
    Block_body body; // Public body (transaction data)

    /** Construct a Block
     *  @param txs transactions to include in the block body
     *  @param prev_hash the hash of the previous block
     *  @param difficulty mining difficulty (the number of leading zeros required for a valid hash)
     */
    Block(const std::vector<Transaction>& txs,
          const std::string& prev_hash,
          uint32_t difficulty);

    /** The hash of this block. */
    const std::string&
    hash() const;

    /** Compute the hash for this block.
     *
     *  This method does not modify the cached hash; it computes a fresh hash
     *  from the data contained in the block header
     */
    std::string
    compute_hash() const;

    /** Perform proof-of-work mining
     * 
     *  Repeatedly iterate nonce and recompute hash until the hash matches the
     *  difficulty criteria.
     */
    void
    mine();

    /** Validate the block. */
    bool
    validate() const;

private:
    std::string hash_;

    /** Compute the Merkle root from the block body */
    std::string
    compute_merkle_root() const;
};

//
// Inline Implementations
//

inline
const std::string&
Block::
hash() const
{
    return hash_;
}

} // namespace bc

#endif // BLOCK_HPP_
