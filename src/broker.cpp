#include <rtx/broker.hpp>
#include <rtx/log.hpp>
#include <rtx/message.hpp>

#include <bln_net/utils.hpp>

namespace rtx {

broker::broker(bln_net::udp::socket& s)
    : m_socket{s}
{}

void broker::process()
{
    using namespace std;

    auto [sender, data] = m_socket.wait();
    auto [head, body] = unpack(bln_net::to_string(data));

    if (head == "sub")
        process(subscribe{move(sender), move(body)});
    else if (head == "unsub")
        process(unsubscribe{move(sender), move(body)});
    else
        process(publish{move(sender), move(head), move(body)});
}

void broker::process(subscribe m)
{
    rtx_log("[s]", m.client.to_string());
    m_topics[std::move(m.topic)].insert(std::move(m.client));
}

void broker::process(const unsubscribe& m)
{
    auto& clients = m_topics[m.topic];

    if (clients.erase(m.client))
        rtx_log("[u]", m.client.to_string());

    if (clients.empty())
        m_topics.erase(m.topic);
}

void broker::process(const publish& m) const
{
    const auto it = m_topics.find(m.topic);
    if (it == m_topics.end())
    {
        rtx_log("[p] unknown topic:", m.topic);
        return;
    }

    auto& clients = it->second;
    const auto bytes = bln_net::to_bytes(m.data);

    for (const auto& c : clients)
    {
        if (c == m.client)
            continue;

        m_socket.put({c, bytes});
        rtx_dbg("[p]", c.to_string(), "|", m.topic, "|", m.data);
    }
}

auto broker::num_topics() const -> u32
{
    return m_topics.size();
}

auto broker::num_clients(const topic& t) const -> u32
{
    const auto it = m_topics.find(t);
    return (it != m_topics.end()) ? m_topics.at(t).size() : 0;
}

} // namespace rtx
