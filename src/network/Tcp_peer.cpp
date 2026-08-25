#include "Tcp_peer.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

namespace bc
{

Tcp_peer::
Tcp_peer(const std::string& host, std::uint16_t port)
: host_(host), port_(port), socket_fd_(-1), connected_(false)
{}

bool
Tcp_peer::
connect_impl()
{
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd_ < 0)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);

    if (inet_pton(AF_INET, host_.c_str(), &addr.sin_addr) <= 0)
        return false;

    if (::connect(socket_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        return false;

    connected_ = true;
    return true;
}

void
Tcp_peer::
disconnect_impl()
{
    if (connected_)
    {
        close(socket_fd_);
        connected_ = false;
    }
}

bool
Tcp_peer::
send_impl(const std::string& data)
{
    if (!connected_)
        return false;

    ssize_t n = ::send(socket_fd_, data.c_str(), data.size(), 0);
    return n == (ssize_t)data.size();
}

std::string
Tcp_peer::
receive_impl()
{
    if (!connected_)
        return {};

    char buffer[4096];
    ssize_t n = ::recv(socket_fd_, buffer, sizeof(buffer), 0);

    if (n <= 0)
        return {};

    return std::string(buffer, n);
}

} // namespace bc
