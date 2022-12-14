#include <rtx/broker.hpp>

#include <bln_net/udp_socket_asio.hpp>

#include <thread>

auto main() -> int
{
    using socket = bln_net::udp::socket_asio;

    socket s{8000};
    rtx::broker b{s};

    std::thread runner{[&](){
        while (true)
            b.process();
    }};

    runner.join();

    return 0;
}
