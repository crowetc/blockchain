#include "crypto_utils.hpp"
#include "Transaction.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace bc
{

Transaction::
Transaction(const std::string& sender,
            const std::string& receiver,
            std::uint64_t amount,
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

    if (amount == 0)
    {
        throw std::invalid_argument("Transaction amount must be positive.");
    }
}

std::string
Transaction::
hash() const
{
    return sha256_hex(serialize());
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
    if (sender_.empty() || receiver_.empty())
        return false;

    if (sender_ == receiver_)
        return false;

    if (amount_ == 0)
        return false;

    // Signature validation disabled until signatures are serialized
    return true;
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

namespace
{

std::string
read_field(const std::string& input,
           std::size_t& pos)
{
    const auto next = input.find('|', pos);

    if (next == std::string::npos)
        throw std::invalid_argument(
            "Invalid transaction serialization."
        );

    std::string field = input.substr(
        pos,
        next - pos
    );

    pos = next + 1;

    return field;
}

} // namespace anonymous

Transaction
Transaction::
deserialize(const std::string& raw)
{
    if (raw.empty())
    {
        throw std::invalid_argument(
            "Invalid transaction serialization."
        );
    }

    std::size_t pos = 0;

    auto read_field =
        [&raw, &pos]() -> std::string
        {
            const std::size_t next =
                raw.find('|', pos);

            if (next == std::string::npos)
            {
                throw std::invalid_argument(
                    "Invalid transaction serialization."
                );
            }

            const std::string field =
                raw.substr(
                    pos,
                    next - pos
                );

            pos = next + 1;

            return field;
        };

    const std::string sender =
        read_field();

    const std::string receiver =
        read_field();

    const std::string amount_string =
        read_field();

    // The timestamp is the remainder.
    const std::string timestamp_string =
        raw.substr(pos);

    if (sender.empty() ||
        receiver.empty() ||
        amount_string.empty() ||
        timestamp_string.empty())
    {
        throw std::invalid_argument(
            "Invalid transaction serialization."
        );
    }

    try
    {
        std::size_t parsed = 0;

        const std::uint64_t amount =
            std::stoull(
                amount_string,
                &parsed
            );

        if (parsed != amount_string.size())
        {
            throw std::invalid_argument(
                "Invalid transaction amount."
            );
        }

        parsed = 0;

        const long long timestamp_value =
            std::stoll(
                timestamp_string,
                &parsed
            );

        if (parsed != timestamp_string.size())
        {
            throw std::invalid_argument(
                "Invalid transaction timestamp."
            );
        }

        const std::time_t timestamp =
            static_cast<std::time_t>(
                timestamp_value
            );

        return Transaction(
            sender,
            receiver,
            amount,
            timestamp
        );
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(
            "Invalid transaction serialization."
        );
    }
}

} // namespace bc
