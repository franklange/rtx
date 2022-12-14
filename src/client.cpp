#include <rtx/client.hpp>
#include <rtx/message.hpp>

#include <bln_net/utils.hpp>

namespace rtx {

namespace {

auto to_bytes(std::string a, std::string b) -> bln_net::bytes
{
    return bln_net::to_bytes(rtx::pack(std::move(a), std::move(b)));
}

} // namespace anonym

client::client(socket& s, endpoint server)
    : m_socket{s}
    , m_server{std::move(server)}
{}

auto client::wait() -> std::string
{
    return bln_net::to_string(m_socket.wait().data);
}

void client::send(std::string s)
{
    m_socket.put({m_server, bln_net::to_bytes(std::move(s))});
}

} // namespace rtx
