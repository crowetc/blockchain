#ifndef MERKLE_PROOF_HPP_
#define MERKLE_PROOF_HPP_

#include <string>
#include <vector>

namespace bc
{

// Forward Declarations
class Merkle_tree;

struct Merkle_proof
{
    std::string leaf_hash;
    std::vector<std::string> siblings;
    std::vector<bool> is_right_sibling;
};

//
// Free Functions
//

Merkle_proof
generate_proof(const Merkle_tree& tree, std::size_t index);

bool
verify(const Merkle_proof& proof, const std::string& root);

} // namespace bc

#endif // MERKLE_PROOF_HPP_
