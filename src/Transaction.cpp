#include "crypto_utils.hpp"
#include "Transaction.hpp"

#include <sstream>

namespace bc
{

Transaction::
Transaction(const std::string& sender,
            const std::string& receiver,
            uint64_t amount,
            std::time_t timestamp)
: sender_(sender),
  receiver_(receiver),
  amount_(amount),
  timestamp_(timestamp)
{
    if (sender.empty() || receiver.empty())
    {
        throw std::invalid_argument("Transaction participants cannot be empty.");
    }

    if (amount <= 0)
    {
        throw std::invalid_argument("Transaction amount must be positive.");
    }
}

std::string
Transaction::
serialize() const
{
    std::stringstream ss;
    ss << sender_ << '|'
       << receiver_ << '|'
       << amount_ << '|'
       << timestamp_;
    return ss.str();
}

void
Transaction::
sign(const std::array<unsigned char, 64>& sk)
{
    // Extract public key from secret key (libsodium stores pk inside sk)
    std::copy(sk.begin() + 32, sk.end(), public_key_.begin());

    signature_ = bc::sign(serialize(), sk);
}

bool
Transaction::
validate() const
{
    // Check structure
    if (sender_.empty() || receiver_.empty()) { return false; }
    if (amount_ <= 0) { return false; }

    // Check signature
    if (signature_.empty()) { return false; }
    return verify(serialize(), signature_, public_key_);
}

} // namespace bc
