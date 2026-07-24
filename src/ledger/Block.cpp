#include "Block.hpp"
#include "crypto_utils.hpp"
#include "Merkle_tree.hpp"

#include <sstream>

namespace bc
{

Block::
Block(const std::vector<Transaction>& txs,
      const std::string& prev_hash,
      uint32_t difficulty)
{
    // Fill body
    body.transactions = txs;

    // Fill header
    header.prev_hash = prev_hash;
    header.time = std::time(nullptr);
    header.nonce = 0;
    header.difficulty = difficulty;

    // Compute Merkle root from body
    header.merkle_root = compute_merkle_root();

    // Initialize hash
    hash_ = compute_hash();
}

std::string
Block::
compute_hash() const
{
    std::stringstream ss;
    ss << header.prev_hash
       << header.merkle_root
       << header.time
       << header.nonce
       << header.difficulty;

    return sha256_hex(ss.str());
}

void
Block::
mine()
{
    std::string target(header.difficulty, '0');

    do
    {
        header.nonce++;
        hash_ = compute_hash();
    } while (hash_.substr(0, header.difficulty) != target);

    std::cout << "Block mined: " << hash_ << '\n';
}

bool
Block::
validate() const
{
    // Check Merkle Root
    if (compute_merkle_root() != header.merkle_root) { return false; }

    // Check Hash
    if (compute_hash() != hash_) { return false; }

    // Check Hash matches Difficulty
    std::string target(header.difficulty, '0');
    if (hash_.substr(0, header.difficulty) != target) { return false; }

    // Check Transactions
    for (const auto& tx : body.transactions)
    {
        if (!tx.validate()) { return false; }
    }

    return true;
}

std::string
Block::
compute_merkle_root() const
{
    if (body.transactions.empty())
    {
        return sha256_hex("");
    }

    Merkle_tree tree(body.transactions);
    return tree.root();
}

} // namespace bc
