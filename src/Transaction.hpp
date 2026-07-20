#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <ctime>
#include <string>
#include <sstream>
#include <stdexcept>

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

    /** Accesors. */
    const std::string&
    sender() const;

    const std::string&
    receiver() const;

    uint64_t
    amount() const;

    std::time_t
    timestamp() const;

    /** Serialize the transaction into a string. */
    std::string
    serialize() const;

    /** Validate the transaction. */
    bool
    validate() const;

private:
    // Core fields
    std::string sender_;
    std::string receiver_;
    uint64_t amount_;
    std::time_t timestamp_;
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

} // namespace bc

#endif // TRANSACTION_HPP_
