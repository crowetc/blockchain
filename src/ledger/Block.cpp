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
serialize() const
{
    std::stringstream ss;

    // Serialize header fields
    ss << header.prev_hash << '|'
       << header.merkle_root << '|'
       << header.time << '|'
       << header.nonce << '|'
       << header.difficulty << '|';

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
    // Split header from transaction list
    auto header_end = raw.find_last_of('|');
    std::string header_part = raw.substr(0, header_end);
    std::string tx_part = raw.substr(header_end + 1);

    // Parse header fields
    std::vector<std::string> fields;
    std::size_t pos = 0, next;
    while ((next = header_part.find('|', pos)) != std::string::npos)
    {
        fields.push_back(header_part.substr(pos, next - pos));
        pos = next + 1;
    }

    Block_header hdr;
    hdr.prev_hash = fields[0];
    hdr.merkle_root = fields[1];
    hdr.time = std::stoll(fields[2]);
    hdr.nonce = std::stoull(fields[3]);
    hdr.difficulty = std::stoul(fields[4]);

    // Parse transactions
    std::vector<Transaction> txs;
    pos = 0;
    while ((next = tx_part.find(';', pos)) != std::string::npos)
    {
        txs.push_back(Transaction::deserialize(tx_part.substr(pos, next - pos)));
        pos = next + 1;
    }

    if (pos < tx_part.size())
        txs.push_back(Transaction::deserialize(tx_part.substr(pos)));

    // Reconstruct block
    Block b(txs, hdr.prev_hash, hdr.difficulty);
    b.header = hdr;
    b.hash_ = b.compute_hash();

    return b;
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
