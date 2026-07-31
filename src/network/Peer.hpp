#ifndef PEER_HPP_
#define PEER_HPP_

namespace bc
{

/** Peer
 *
 *  Represents a remote node in the blockchain network.
 *
 *  Responsibilities:
 *    - Establish and maintain a TCP connection to a remote host.
 *    - Send and receive raw string data over the socket.
 *    - Provide a unique identifier for use in peer maps.
 *
 *  This class abstracts low-level socket operations so that higher-level
 *  components (e.g., Node) can treat peers as simple communication endpoints.
 */
class Peer
{
public:
    /** Construct a Peer
     *  @param host Remote hostname or IP address.
     *  @param port Remote port number.
     *
     *  The peer is not connected upon construction; connect() must be called
     *  explicitly.
     */
    Peer(const std::string& host, uint16_t port);

    /** Establish a TCP connection to the remote peer.
     *
     *  @return true if the connection succeeds, false otherwise.
     */
    bool
    connect();

    /** Close the TCP connection if one is active. */
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

    /** Unique identifier for this peer.
     *
     *  Format: "<host>:<port>"
     *
     *  Used by Node to index peers in an unordered_map.
     */
    std::string
    id() const;

private:
    std::string host_;
    uint16_t port_;
    int socket_fd_;
    bool connected_;
};

//
// Inline Implementations
//

inline
bool
Peer::
connected() const
{
    return connected_;
}

inline
std::string
Peer::
id() const
{
    return host_ + ':' + std::to_string(port_);
}

} // namespace bc

#endif // PEER_HPP_
