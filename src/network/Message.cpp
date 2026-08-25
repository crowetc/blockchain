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
    const std::size_t pos = raw.find('|');

    // A valid message must contain a type/payload delimiter.
    if (pos == std::string::npos)
    {
        throw std::invalid_argument(
            "Invalid message: missing delimiter."
        );
    }

    // Extract the type field.
    const std::string type_str =
        raw.substr(0, pos);

    if (type_str.empty())
    {
        throw std::invalid_argument(
            "Invalid message: empty type field."
        );
    }

    int type_int;

    try
    {
        std::size_t parsed = 0;

        type_int = std::stoi(
            type_str,
            &parsed
        );

        // std::stoi("1abc") succeeds unless we verify that the
        // entire field was consumed.
        if (parsed != type_str.size())
        {
            throw std::invalid_argument(
                "Invalid message type."
            );
        }
    }
    catch (const std::exception&)
    {
        throw std::invalid_argument(
            "Invalid message type."
        );
    }

    Message_type type;

    switch (type_int)
    {
        case static_cast<int>(
            Message_type::NEW_TRANSACTION
        ):
            type = Message_type::NEW_TRANSACTION;
            break;

        case static_cast<int>(
            Message_type::NEW_BLOCK
        ):
            type = Message_type::NEW_BLOCK;
            break;

        default:
            throw std::invalid_argument(
                "Invalid message type."
            );
    }

    // Everything after the FIRST delimiter is the payload.
    // This preserves payloads containing '|'.
    const std::string payload =
        raw.substr(pos + 1);

    return Message(type, payload);
}

} // namespace bc
