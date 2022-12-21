#pragma once

#include <rtx/message.hpp>
#include <rtx/types.hpp>

#include <optional>
#include <string>

namespace rtx {

class broker
{
public:
    broker(socket&);

    void process();

    void process(packet);
    void process(heartbeat);
    void process(subscribe);
    void process(const unsubscribe&);
    void process(const publish&) const;

    auto num_topics() const -> u32;
    auto num_clients(const topic&) const -> u32;

    auto collect() -> remotes;
    void trim(const remotes&);

    auto wait() -> std::optional<packet>;

private:
    const u8 m_maxBeats{3u};
    const timeout m_cadence{5s};

    socket& m_socket;
    topics m_topics;

    hbeats m_beats{};
    timeout m_waited{0};
};

} // namespace rtx
