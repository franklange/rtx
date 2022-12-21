#include <rtx/broker.hpp>
#include <rtx/log.hpp>
#include <rtx/message.hpp>

#include <bln_net/utils.hpp>

#include <algorithm>

namespace rtx {

broker::broker(bln_net::udp::socket& s)
    : m_socket{s}
{}

void broker::process()
{
    auto p = wait();
    p ? process(std::move(p.value())) : trim(collect());
}

void broker::process(packet p)
{
    using namespace std;

    auto& [sender, data] = p;
    auto [head, body] = unpack(bln_net::to_string(data));

    if (head == "hb")
        process(heartbeat{move(sender)});
    else if (head == "sub")
        process(subscribe{sender, move(body)});
    else if (head == "unsub")
        process(unsubscribe{move(sender), move(body)});
    else
        process(publish{move(sender), move(head), move(body)});
}

void broker::process(heartbeat m)
{
    const auto it = m_beats.find(m.remote);
    if (it == m_beats.end())
    {
        rtx_dbg("[h] unknown client:", m.remote.to_string());
        return;
    }

    rtx_dbg("[h]", m.remote.to_string());

    auto& b = m_beats[std::move(m.remote)];
    b = std::min(m_maxBeats, static_cast<u8>(b + 1u));
}

void broker::process(subscribe m)
{
    rtx_log("[s]", m.client.to_string());
    m_topics[std::move(m.topic)].insert(m.client);
    m_beats[std::move(m.client)] = m_maxBeats;
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

auto broker::collect() -> remotes
{
    remotes res;

    for (auto& [c, hb] : m_beats)
    {
        hb--;
        if (!hb) res.insert(c);
    }

    rtx_dbg("[c]", res.size());
    return res;
}

void broker::trim(const remotes& remotes)
{
    for (const auto& r : remotes)
    {
        m_beats.erase(r);

        for (auto it = m_topics.begin(); it != m_topics.end();)
        {
            it->second.erase(r);
            if (it->second.empty())
                it = m_topics.erase(it);
            else
                ++it;
        }

        rtx_log("[t]", r.to_string());
    }
}

auto broker::wait() -> std::optional<packet>
{
    const auto p = m_socket.measured_wait(m_cadence - m_waited);

    m_waited += m_socket.last_wait();

    if (m_waited >= (m_cadence - 1ms))
        m_waited = 0ms;

    return p;
}

} // namespace rtx
