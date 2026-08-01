#ifndef TCP_PEER_HPP_
#define TCP_PEER_HPP_

#include "Peer.hpp"

#include <string>

namespace bc
{

/** TCP Peer
 *
 *  Concrete Peer implementation using POSIX TCP sockets.
 *
 *  Implements the Peer interface using a blocking, stream-oriented
 *  TCP connection. All operations map directly to standard socket
 *  calls (connect, send, recv, close).
 */
class Tcp_peer : public Peer
{
public:
    /** Construct a TCP Peer
     *  @param host Remote hostname or IP address.
     *  @param port Remote port number.
     *
     *  The peer is not connected upon construction; connect() must be called
     *  explicitly.
     */
    Tcp_peer(const std::string& host, uint16_t port);

private:
    std::string host_;
    uint16_t port_;
    int socket_fd_;
    bool connected_;

    /** Establish a TCP connection using POSIX socket APIs.
     *
     *  Creates a socket, resolves the address, and performs a blocking
     *  connect(). Returns true only if the connection is successfully
     *  established.
     */
    bool
    connect_impl() final;

    /** Close the TCP socket if connected. */
    void
    disconnect_impl() final;

    /** Send data using a blocking TCP send().
     *
     *  Returns true only if the entire buffer is written. Partial writes
     *  are treated as failure for simplicity.
     */
    bool
    send_impl(const std::string& data) final;

    /** Receive data using a blocking TCP recv().
     *
     *  Returns an empty string if no data is available or if the peer
     *  disconnects.
     */
    std::string
    receive_impl() final;

    /** Return true if the TCP connection is active. */
    bool
    connected_impl() const final;

    /** Identifier in the form "host:port". */
    std::string
    id_impl() const final;
};

//
// Inline Implementations
//

inline
bool
Tcp_peer::
connected_impl() const
{
    return connected_;
}

inline
std::string
Tcp_peer::
id_impl() const
{
    return host_ + ':' + std::to_string(port_);
}

} // namespace bc

#endif // TCP_PEER_HPP_
