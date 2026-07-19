#ifndef MERKLE_TREE_HPP_
#define MERKLE_TREE_HPP_

#include <string>
#include <vector>

#include "Transaction.hpp"

namespace bc
{

/** Merkle tree
 *
 *  A binary hash tree for a set of transactions where:
 *  - leaves are transaction hashes
 *  - subsequent levels are hashes of concatenated child hashes
 *  - the root commits the entire set of transactions
 */
class Merkle_tree
{
public:
    using level_t = std::vector<std::string>;

    /** Construct a Merkle_tree
     *  @param txs the set of transactiosn to store in the tree
     */
    Merkle_tree(const std::vector<Transaction>& txs);

    /** The Merkle root for this tree. */
    const std::string&
    root() const;

    /** All the levels of the tree. */
    const std::vector<level_t>&
    levels() const;

private:
    std::string root_;
    std::vector<level_t> levels_;

    /** Build a Merkle tree from the given transactions. */
    void
    build(const std::vector<Transaction>& txs);
};

//
// Inline Implementations
//

inline
const std::string&
Merkle_tree::
root() const
{
    return root_;
}

inline
const std::vector<std::vector<std::string>>&
Merkle_tree::
levels() const
{
    return levels_;
}

} // namespace bc

#endif // MERKLE_TREE_HPP_
