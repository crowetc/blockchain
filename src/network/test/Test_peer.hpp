#ifndef TEST_PEER_HPP_
#define TEST_PEER_HPP_

#include "Peer.hpp"
#include <string>
#include <vector>

namespace bc
{

/** Test_peer
 *
 *  In-memory Peer implementation used for deterministic unit testing.
 *
 *  Behaves like a connected peer but stores all sent messages in a buffer
 *  and allows tests to inject inbound messages directly. No real networking
 *  is performed.
 */
class Test_peer : public Peer
{
public:

    Test_peer(const std::string& host, std::uint16_t port)
        : host_(host), port_(port), connected_(true)
    {}

    //
    // Test utilities
    //

    /** Inject a raw message into the receive buffer. */
    void
    push_inbox(const std::string& raw)
    {
        inbox_.push_back(raw);
    }

    /** Access messages sent by Node for assertions. */
    const std::vector<std::string>&
    sent() const
    {
        return sent_;
    }

private:
    std::string host_;
    std::uint16_t port_;
    bool connected_;

    std::vector<std::string> inbox_;
    std::vector<std::string> sent_;

    //
    // Peer interface implementation
    //

    bool
    connect_impl() final
    {
        connected_ = true;
        return true;
    }

    void
    disconnect_impl() final
    {
        connected_ = false;
    }

    bool
    send_impl(const std::string& data) final
    {
        sent_.push_back(data);
        return true;
    }

    std::string
    receive_impl() final
    {
        if (inbox_.empty())
            return "";

        std::string msg = inbox_.front();
        inbox_.erase(inbox_.begin());
        return msg;
    }

    bool
    connected_impl() const final
    {
        return connected_;
    }

    std::string
    id_impl() const final
    {
        return host_ + ":" + std::to_string(port_);
    }
};

} // namespace bc

#endif // TEST_PEER_HPP_
