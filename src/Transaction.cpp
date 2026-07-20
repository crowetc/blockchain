#include "sha256.h"
#include "Transaction.hpp"

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
    ss << "sender: " << sender_
       << " | receiver: " << receiver_
       << " | amount: " << amount_
       << " | time: " << timestamp_;
    return ss.str();
}

bool
Transaction::
validate() const
{
    // Check structure
    if (sender_.empty() || receiver_.empty()) { return false; }
    if (amount_ <= 0) { return false; }

    return true;
}

} // namespace bc
