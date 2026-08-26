#include "crypto_utils.hpp"
#include "Transaction.hpp"

#include <sodium.h>

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace bc
{

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

std::uint64_t
parse_uint64(const std::string& value)
{
    std::size_t parsed = 0;

    const std::uint64_t result = std::stoull(value, &parsed);

    if (parsed != value.size())
    {
        throw std::invalid_argument(
            "Invalid unsigned integer."
        );
    }

    return result;
}

long long
parse_integer(const std::string& value)
{
    std::size_t parsed = 0;

    const long long result = std::stoll(value, &parsed);

    if (parsed != value.size())
    {
        throw std::invalid_argument(
            "Invalid integer."
        );
    }

    return result;
}

} // namespace anonymous

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
    if (sender_.empty() || receiver_.empty())
    {
        throw std::invalid_argument(
            "Transaction participants cannot be empty."
        );
    }

    if (amount_ == 0)
    {
        throw std::invalid_argument(
            "Transaction amount must be positive."
        );
    }
}

std::string
Transaction::
hash() const
{
    return sha256_hex(serialize());
}

std::string
Transaction::
signing_payload() const
{
    std::stringstream ss;

    ss << sender_ << '|'
       << receiver_ << '|'
       << amount_ << '|'
       << timestamp_ << '|'
       << to_hex(
              public_key_.data(),
              public_key_.size()
          );

    return ss.str();
}

void
Transaction::
sign(const std::array<unsigned char, 64>& sk)
{
    // Extract public key from secret key (libsodium stores pk inside sk)
    std::copy(sk.begin() + 32, sk.end(), public_key_.begin());

    signature_ = bc::sign(signing_payload(), sk);
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

    if (signature_.size() != crypto_sign_BYTES)
        return false;
    
    return verify(signing_payload(), signature_, public_key_);
}

std::string
Transaction::
serialize() const
{
    std::stringstream ss;
    ss << sender_    << '|'
       << receiver_  << '|'
       << amount_    << '|'
       << timestamp_ << '|'
       << to_hex(public_key_.data(), public_key_.size()) << '|'
       << to_hex(signature_.data(), signature_.size());
    return ss.str();
}

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

    try
    {
        std::size_t pos = 0;

        const std::string sender = read_field(raw, pos);
        const std::string receiver = read_field(raw, pos);
        const std::string amount_string = read_field(raw, pos);
        const std::string timestamp_string = read_field(raw, pos);
        const std::string pk_hex = read_field(raw, pos);
        const std::string sig_hex = raw.substr(pos);

        if (sender.empty() ||
            receiver.empty() ||
            amount_string.empty() ||
            timestamp_string.empty() ||
            pk_hex.empty() || sig_hex.empty())
        {
            throw std::invalid_argument(
                "Invalid transaction serialization."
            );
        }

        const auto amount = parse_uint64(amount_string);
        const auto timestamp_value = parse_integer(timestamp_string);
        auto pk_vec = bc::from_hex(pk_hex);
        auto sig_vec = bc::from_hex(sig_hex);

        if (pk_vec.size() != crypto_sign_PUBLICKEYBYTES)
        {
            throw std::invalid_argument(
                "Invalid public key size."
            );
        }

        if (sig_vec.size() != crypto_sign_BYTES)
        {
            throw std::invalid_argument(
                "Invalid signature size."
            );
        }

        Transaction tx(
            sender,
            receiver,
            amount,
            static_cast<std::time_t>(timestamp_value)
        );

        std::copy(pk_vec.begin(), pk_vec.end(), tx.public_key_.begin());
        tx.signature_ = std::move(sig_vec);

        return tx;
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(
            "Invalid transaction serialization."
        );
    }
}

} // namespace bc
