#pragma once

#include <rtx/types.hpp>
#include <string>

namespace rtx {

struct message
{
    std::string head;
    std::string body;

    auto operator<=>(const message&) const = default;
};

auto pack(const std::string&, const std::string&) -> std::string;
auto unpack(const std::string&) -> message;

struct heartbeat
{
    rtx::remote remote;
};

struct subscribe
{
    rtx::remote client;
    rtx::topic  topic;
};

struct unsubscribe
{
    rtx::remote client;
    rtx::topic  topic;
};

struct publish
{
    rtx::remote client;
    rtx::topic  topic;
    std::string data;
};

} // namespace rtx
