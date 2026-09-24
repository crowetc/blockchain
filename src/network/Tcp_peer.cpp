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
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
        return false;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);

    if (::inet_pton(AF_INET, host_.c_str(), &addr.sin_addr) <= 0)
    {
        ::close(fd);
        return false;
    }

    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        ::close(fd);
        return false;
    }

    socket_fd_ = fd;
    connected_.store(true);

    return true;
}

void
Tcp_peer::
disconnect_impl()
{
    std::lock_guard<std::mutex> lock(socket_mutex_);

    if (!connected_.exchange(false))
        return;

    ::shutdown(socket_fd_, SHUT_RDWR);
    ::close(socket_fd_);
    socket_fd_ = -1;
}

bool
Tcp_peer::
send_impl(const std::string& data)
{
    int fd;

    {
        std::lock_guard<std::mutex> lock(socket_mutex_);

        if (!connected_.load())
            return false;

        fd = ::dup(socket_fd_);

        if (fd < 0)
            return false;
    }

    std::size_t total_sent = 0;

    while (total_sent < data.size())
    {
        ssize_t n = ::send(
            fd,
            data.data() + total_sent,
            data.size() - total_sent,
            MSG_NOSIGNAL);

        if (n > 0)
        {
            total_sent += static_cast<std::size_t>(n);
            continue;
        }

        if (n < 0 && errno == EINTR)
            continue;

        ::close(fd);
        return false;
    }

    ::close(fd);
    return true;
}

std::string
Tcp_peer::
receive_impl()
{
    int fd;

    {
        std::lock_guard<std::mutex> lock(socket_mutex_);

        if (!connected_.load())
            return {};

        fd = ::dup(socket_fd_);

        if (fd < 0)
            return {};
    }

    char buffer[4096];
    ssize_t n = ::recv(fd, buffer, sizeof(buffer), 0);

    ::close(fd);

    if (n <= 0)
        return {};

    return std::string(buffer, n);
}

} // namespace bc
