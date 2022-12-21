#pragma once

#include <rtx/types.hpp>
#include <bln_net/udp_socket.hpp>

#include <atomic>
#include <string>
#include <thread>

namespace rtx {

class client
{
    using socket = bln_net::udp::socket;
    using endpoint = bln_net::udp::endpoint;
    using topics = std::unordered_set<topic>;

public:
    client(socket&, endpoint);
    ~client();

    void send(std::string);
    auto wait() -> std::string;

private:
    void send_heartbeat();

    socket& m_socket;
    const endpoint m_server;

    topics m_topics;

    std::thread m_thread;
    std::atomic_bool m_run{true};
};

} // namespace rtx
