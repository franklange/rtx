#include <rtx/client.hpp>
#include <rtx/log.hpp>
#include <rtx/message.hpp>

#include <bln_net/utils.hpp>

#include <chrono>

namespace rtx {

client::client(socket& s, endpoint server)
    : m_socket{s}
    , m_server{std::move(server)}
    , m_thread{&client::send_heartbeat, this}
{}

client::~client()
{
    m_run = false;

    if (m_thread.joinable())
        m_thread.join();
}

auto client::wait() -> std::string
{
    return bln_net::to_string(m_socket.wait().data);
}

void client::send(std::string s)
{
    const auto m = unpack(s);
    if (m.head == "sub")
        m_topics.insert(m.body);
    else if (m.head == "unsub")
        m_topics.erase(m.body);

    m_socket.put({m_server, bln_net::to_bytes(std::move(s))});
}

void client::send_heartbeat()
{
    using namespace std::chrono_literals;
    static const bln_net::bytes hb{bln_net::to_bytes("hb|")};

    while (m_run)
    {
        if (!m_topics.empty())
        {
            m_socket.put({m_server, hb});
            rtx_dbg("[h]");
        }
        std::this_thread::sleep_for(1s);
    }
}

} // namespace rtx
