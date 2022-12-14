#pragma once

#include <rtx/types.hpp>
#include <bln_net/udp_socket.hpp>

#include <string>

namespace rtx {

class client
{
    using socket = bln_net::udp::socket;
    using endpoint = bln_net::udp::endpoint;

public:
    client(socket&, endpoint);

    void send(std::string);
    auto wait() -> std::string;

private:
    socket& m_socket;
    const endpoint m_server;
};

} // namespace rtx
