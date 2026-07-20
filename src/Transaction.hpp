#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <string>
#include <sstream>

namespace bc
{

/** Transaction
 * 
 *  A simple structure that represents the transfer of value between two parties.
 */
struct Transaction
{
    std::string from; // sender
    std::string to;   // recipient
    uint64_t amount;  // transferred value
    time_t time;      // time of transaction

    /** Serialize the transaction into a string. */
    std::string
    serialize() const
    {
        std::stringstream ss;
        ss << "from:" << from
           << "|to:" << to
           << "|amount:" << amount
           << "|time:" << time;
        return ss.str();
    }

    /** Validate the transaction. */
    bool
    validate() const
    {
        // Check structure
        if (from.empty() || to.empty()) { return false; }
        if (amount <= 0) { return false; }

        return true;
    }

};

} // namespace bc

#endif // TRANSACTION_HPP_