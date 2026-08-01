#ifndef PEER_HPP_
#define PEER_HPP_

#include <string>
#include <cstdint>

namespace bc
{

/** Peer
 *
 *  Protocol-agnostic communication interface used by Node.
 *
 *  Responsibilities:
 *  - Establish and maintain a connection to a remote host.
 *  - Send and receive raw string data over the connection.
 *  - Provide a unique identifier for use in peer maps.
 */
class Peer
{
public:
    virtual ~Peer() = default;

    /** Establish a connection to the remote peer.
     *  @return true if the connection succeeds, false otherwise.
     */
    bool
    connect();

    /** Close the connection if one is active. */
    void
    disconnect();

    /** Send raw string data to the peer.
     *  @param data The encoded message payload.
     *  @return @c true if the send operation succeeds, @c false otherwise.
     */
    bool
    send(const std::string& data);

    /** Receive raw string data from the peer.
     *  @return The received data, or an empty string if no data is available.
     */
    std::string
    receive();

    /** Check whether the peer is currently connected. */
    bool
    connected() const;

    /** Unique identifier for this peer (e.g., "host:port"). */
    std::string
    id() const;

private:

    virtual bool
    connect_impl() = 0;

    virtual void
    disconnect_impl() = 0;

    virtual bool
    send_impl(const std::string& data) = 0;

    virtual std::string
    receive_impl() = 0;

    virtual bool
    connected_impl() const = 0;

    virtual std::string
    id_impl() const = 0;
};

//
// Inline Implementations
//

inline
bool
Peer::
connect()
{
    return connect_impl();
}

inline
void
Peer::
disconnect()
{
    disconnect_impl();
}

inline
bool
Peer::
send(const std::string& data)
{
    return send_impl(data);
}

inline
std::string
Peer::
receive()
{
    return receive_impl();
}

inline
bool
Peer::
connected() const
{
    return connected_impl();
}

inline
std::string
Peer::
id() const
{
    return id_impl();
}

} // namespace bc

#endif // PEER_HPP_
