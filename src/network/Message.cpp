#include "Message.hpp"

#include <stdexcept>

namespace bc
{

Message::
Message(Message_type type, const std::string& payload)
: type(type), payload(payload)
{}

std::string
Message::
encode() const
{
    return std::to_string(static_cast<int>(type)) + "|" + payload;
}

Message
Message::
decode(const std::string& raw)
{
    // Find delimiter
    std::size_t pos = raw.find('|');
    if (pos == std::string::npos)
    {
        // Malformed: no delimiter
        return Message(Message_type::NEW_TRANSACTION, "");
    }

    // Extract type field
    std::string type_str = raw.substr(0, pos);
    int type_int = 0;

    try
    {
        type_int = std::stoi(type_str);
    }
    catch (...)
    {
        // Malformed type field
        return Message(Message_type::NEW_TRANSACTION, "");
    }

    // Convert integer to enum
    Message_type type = Message_type::NEW_TRANSACTION;
    if (type_int == static_cast<int>(Message_type::NEW_BLOCK))
        type = Message_type::NEW_BLOCK;

    // Extract payload
    std::string payload = raw.substr(pos + 1);

    return Message(type, payload);
}

} // namespace bc
