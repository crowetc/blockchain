#ifndef TRANSACTION_HPP_
#define TRANSACTION_HPP_

#include <string>
#include <sstream>

namespace bc
{

struct Transaction
{
    std::string from;
    std::string to;
    uint64_t amount;
    time_t time;

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