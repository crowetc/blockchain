#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <array>
#include <ctime>
#include <string>
#include <vector>

namespace bc
{

/** Transaction
 * 
 *  A simple structure that represents the transfer of value between two parties.
 */
class Transaction
{
public:
    Transaction(const std::string& sender,
                const std::string& receiver,
                uint64_t amount,
                std::time_t timestamp = std::time(nullptr));

    //
    // Accesors
    //

    const std::string&
    sender() const;

    const std::string&
    receiver() const;

    uint64_t
    amount() const;

    std::time_t
    timestamp() const;

    const std::array<unsigned char, 32>&
    public_key() const;

    const std::vector<unsigned char>&
    signature() const;

    //
    // Signing and Validation
    //

    /** Serialize the transaction into a string. */
    std::string
    serialize() const;

    /** Sign the transaction using the provided Ed25519 secret key. */
    void
    sign(const std::array<unsigned char, 64>& sk);

    /** Validate the transaction. */
    bool
    validate() const;

private:
    // Core fields
    std::string sender_;
    std::string receiver_;
    uint64_t amount_;
    std::time_t timestamp_;

    // Cryptographic fields
    std::array<unsigned char, 32> public_key_{};
    std::vector<unsigned char> signature_; 
};

//
// Inline Implementations
//

inline
const std::string&
Transaction::
sender() const
{
    return sender_;
}

inline
const std::string&
Transaction::
receiver() const
{
    return receiver_;
}

inline
uint64_t
Transaction::
amount() const
{
    return amount_;
}

inline
std::time_t
Transaction::
timestamp() const
{
    return timestamp_;
}

inline
const std::array<unsigned char, 32>&
Transaction::
public_key() const
{
    return public_key_;
}

inline
const std::vector<unsigned char>&
Transaction::
signature() const
{
    return signature_;
}

} // namespace bc

#endif // TRANSACTION_HPP_
