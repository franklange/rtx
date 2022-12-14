#include <rtx/message.hpp>

#include <exception>
#include <sstream>

namespace rtx {

auto pack(const std::string& topic, const std::string& msg) -> std::string
{
    if (topic.empty())
        throw std::runtime_error{"invalid topic"};

    if (msg.empty())
        throw std::runtime_error{"invalid msg"};

    return topic + "|" + msg;
}

auto unpack(const std::string& s) -> message
{
    message res;

    std::istringstream stream{s};
    std::getline(stream, res.head, '|');
    std::getline(stream, res.body);

    return res;
}

} // namespace rtx
