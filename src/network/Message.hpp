#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>

namespace bc
{

/** Message_type
 *
 *  Enumerates all message categories exchanged between nodes.
 *
 *  NEW_TRANSACTION — Broadcast when a node submits a new transaction.
 *  NEW_BLOCK — Broadcast when a node mines or receives a new block.
 *
 *  These message types allow nodes to distinguish how incoming payloads
 *  should be interpreted and processed.
 */
enum class Message_type
{
    NEW_TRANSACTION,
    NEW_BLOCK
};

/** Message
 *
 *  Represents a network message exchanged between peers.
 *
 *  A message consists of:
 *    - A Message_type indicating the semantic meaning.
 *    - A payload string containing the encoded data.
 *
 *  Messages are serialized into a simple delimiter-based string format
 *  for transmission over TCP sockets. The encode() and decode() methods
 *  provide symmetric conversion between structured Message objects and
 *  raw string data.
 */
class Message
{
public:
    /** Construct a Message
     *  @param type The message category.
     *  @param payload The encoded message payload.
     */
    Message(Message_type type, const std::string& payload);

    /** Encode the message into a raw string for transmission.
     *
     *  Format: "<type>|<payload>"
     *
     *  The type is encoded as an integer corresponding to the Message_type
     *  enumeration. The payload is included verbatim.
     */
    std::string
    encode() const;

    /** Decode a raw string into a Message object.
     *  @param raw The encoded message string.
     *  @return A reconstructed Message instance.
     *
     *  Inverse of encode(). Splits the raw string at the first delimiter
     *  and reconstructs the Message_type and payload.
     */
    static Message
    decode(const std::string& raw);

    Message_type type;
    std::string payload;
};

}

#endif // MESSAGE_HPP_
