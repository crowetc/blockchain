#include "sha256.h"
#include "Merkle_proof.hpp"
#include "Merkle_tree.hpp"

namespace bc
{

Merkle_proof
generate_proof(const Merkle_tree& tree, std::size_t index)
{
    Merkle_proof proof;
    proof.leaf_hash = tree.levels()[0][index];

    std::size_t idx = index;

    for (std::size_t level = 0; level < tree.levels().size() - 1; ++level)
    {
        const auto& nodes = tree.levels()[level];

        std::size_t sibling_index = (idx % 2 == 0) ? idx + 1 : idx -1;

        if (sibling_index < nodes.size())
        {
            proof.siblings.push_back(nodes[sibling_index]);
            proof.is_right_sibling.push_back(idx %2 == 0);
        }
        else
        {
            proof.siblings.push_back(nodes[sibling_index]);
            proof.is_right_sibling.push_back(true);
        }

        idx /= 2;
    }

    return proof;
}

bool
verify(const Merkle_proof& proof, const std::string& root)
{
    std::string hash_val = proof.leaf_hash;

    for (std::size_t i = 0; i < proof.siblings.size(); ++i)
    {
        if (proof.is_right_sibling[i])
        {
            hash_val = sha256(hash_val + proof.siblings[i]);
        }
        else
        {
            hash_val = sha256(proof.siblings[i] + hash_val);
        }
    }

    return hash_val == root;
}

} // namespace bc
