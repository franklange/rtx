#pragma once

#include <rtx/message.hpp>
#include <rtx/types.hpp>

#include <string>

namespace rtx {

class broker
{
public:
    broker(socket&);

    void process();

    void process(subscribe);
    void process(const unsubscribe&);
    void process(const publish&) const;

    auto num_topics() const -> u32;
    auto num_clients(const topic&) const -> u32;

private:
    socket& m_socket;
    topics m_topics;
};

} // namespace rtx
