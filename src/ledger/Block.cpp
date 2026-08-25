#include "Block.hpp"
#include "crypto_utils.hpp"
#include "Merkle_tree.hpp"
#include "Transaction.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace bc
{

Block::
Block(const std::vector<Transaction>& txs,
      const std::string& prev_hash,
      std::uint32_t difficulty)
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
    std::ostringstream ss;
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

    while (true)
    {
        header.nonce++;
        hash_ = compute_hash();

        if (hash_.compare(0, header.difficulty, target) == 0)
            break;
    }

    std::cout << "Block mined: " << hash_ << '\n';
}

bool
Block::
validate() const
{
    // Check Merkle Root
    if (compute_merkle_root() != header.merkle_root)
        return false;

    // Check Hash
    if (compute_hash() != hash_)
        return false;

    // Check Hash matches Difficulty
    const std::string target(header.difficulty, '0');
    if (hash_.compare(0, header.difficulty, target) != 0)
        return false;

    // Check Transactions
    for (const auto& tx : body.transactions)
        if (!tx.validate())
            return false;

    return true;
}

std::string
Block::
serialize() const
{
    std::ostringstream ss;

    // Serialize header fields
    ss << header.prev_hash   << '|'
       << header.merkle_root << '|'
       << header.time        << '|'
       << header.nonce       << '|'
       << header.difficulty  << '|';

    // Serialize transactions (semicolon-separated)
    for (std::size_t i = 0; i < body.transactions.size(); ++i)
    {
        ss << body.transactions[i].serialize();
        if (i + 1 < body.transactions.size())
            ss << ';';
    }

    return ss.str();
}

Block
Block::
deserialize(const std::string& raw)
{
    // Extract header section (5 fields)
    std::size_t pos = 0;
    std::size_t next = 0;
    std::vector<std::string> fields;
    fields.reserve(5);

    for (int i = 0; i < 5; ++i)
    {
        next = raw.find('|', pos);
        if (next == std::string::npos)
            throw std::invalid_argument("Invalid block serialization.");

        fields.emplace_back(raw.substr(pos, next - pos));
        pos = next + 1;
    }

    // Remaining part is transaction list
    const std::string tx_part = raw.substr(pos);

    Block_header hdr;
    try
    {
        hdr.prev_hash   = fields[0];
        hdr.merkle_root = fields[1];
        hdr.time        = std::stoll(fields[2]);
        hdr.nonce       = std::stoull(fields[3]);
        hdr.difficulty  = std::stoul(fields[4]);
    }
    catch (...)
    {
        throw std::invalid_argument("Invalid block serialization.");
    }

    // Parse transactions
    std::vector<Transaction> txs;
    pos = 0;

    while ((next = tx_part.find(';', pos)) != std::string::npos)
    {
        txs.emplace_back(Transaction::deserialize(tx_part.substr(pos, next - pos)));
        pos = next + 1;
    }

    if (pos < tx_part.size())
        txs.emplace_back(Transaction::deserialize(tx_part.substr(pos)));

    // Reconstruct block
    Block block(txs, hdr.prev_hash, hdr.difficulty);
    block.header = hdr;
    block.hash_  = block.compute_hash();

    return block;
}

std::string
Block::
compute_merkle_root() const
{
    if (body.transactions.empty())
        return sha256_hex("");

    Merkle_tree tree(body.transactions);
    return tree.root();
}

} // namespace bc
